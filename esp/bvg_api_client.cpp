#include <chrono>
#include <ctime>
#include <esp_http_client.h>
#include <esp_log.h>
#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "bvg_api_client.hpp"
#include "nvs_engine.hpp"
#include "time.hpp"

static const char *TAG = "BvgApiClient";

const std::vector<std::string> ALL_PRODUCTS = {"suburban", "subway", "tram", "bus", "ferry", "express", "regional"};

class HTTPInputStream : public std::streambuf {
  public:
    HTTPInputStream(esp_http_client_handle_t client) : client(client) {
        setg(buffer, buffer, buffer); // Set get pointers to the beginning
    }

  protected:
    // TODO This works, but there are still timeout issues from time to time and it's kinda slow (maybe opening / closing
    // the connection is too expensive?) Yes it is
    int_type underflow() override {
        ESP_LOGI(TAG, "underflowstart");
        if (this->client == nullptr) {
            ESP_LOGE(TAG, "Tried to read from stream before client was initialized");
            return traits_type::eof();
        }

        if (this->isOpen == false) {
            esp_err_t err = esp_http_client_open(client, 0);
            if (err != ESP_OK) {
                ESP_LOGE(TAG, "Failed to open HTTP connection");
                return traits_type::eof(); // End of stream
            }
            this->isOpen = true;
            ESP_LOGI(TAG, "HTTP connection opened");

            auto content_length = esp_http_client_fetch_headers(client);
            if (content_length < 0) {
                ESP_LOGE(TAG, "HTTP client fetch headers failed");
                return traits_type::eof(); // End of stream
            }
        }

        if (gptr() < egptr()) {
            return traits_type::to_int_type(*gptr());
        }

        int bytes_read = esp_http_client_read(client, buffer, BUFFER_SIZE);
        ESP_LOGI(TAG, "Read %d bytes", bytes_read);
        if (bytes_read <= 0) {
            ESP_LOGE(TAG, "Error reading from HTTP client");
            return traits_type::eof(); // End of stream
        }

        if (bytes_read < BUFFER_SIZE) {
            ESP_LOGI(TAG, "Closing HTTP client");
            esp_http_client_close(client);
        }

        setg(buffer, buffer, buffer + bytes_read);

        return traits_type::to_int_type(*gptr());
    }

  private:
    esp_http_client_handle_t client;
    bool isOpen = false;
    static const constexpr size_t BUFFER_SIZE = 1024 * 10;
    char buffer[BUFFER_SIZE];
};

BvgApiClient::BvgApiClient(const std::string &stationId) {
    esp_http_client_config_t config = {
        .url = "https://www.google.com", // Set later
        .user_agent = "DepMon gasparini.lorenzo@gmail.com",
        .timeout_ms = 10000, // Seems to help with timeout issues
        .is_async = false,
    };
    client = esp_http_client_init(&config);
    esp_http_client_set_method(client, HTTP_METHOD_GET);
    esp_http_client_set_header(client, "Accept", "application/json");
}

BvgApiClient::~BvgApiClient() { esp_http_client_cleanup(client); }

void BvgApiClient::setUrl(const std::string &stationId, const std::vector<std::string> &enabledProducts) {
    // TODO store and read number of results from NVS. keep in mind that memory is limited.
    std::map<std::string, std::string> queryParams = {
        {"results", std::to_string(N_RESULTS)},
        {"pretty", "false"},
        // TODO Extract to constant
        {"duration", std::to_string(60)},
    };
    for (const auto &product : ALL_PRODUCTS) {
        queryParams[product] = "false";
    }
    for (const auto &product : enabledProducts) {
        queryParams[product] = "true";
    }

    std::ostringstream url;

    url << "https://v6.bvg.transport.rest/stops/" << stationId << "/departures?";

    for (auto entry = queryParams.begin(); entry != queryParams.end(); ++entry) {
        if (entry != queryParams.begin()) {
            url << "&";
        }

        // TODO URLEncode / escape? We don't really need it
        url << entry->first << "=" << entry->second;
    }

    esp_http_client_set_url(client, url.str().c_str());
}

std::vector<Trip> BvgApiClient::fetchAndParseTrips(const std::string &stationId,
                                                   const std::vector<std::string> &enabledProducts) {
    this->setUrl(stationId, enabledProducts);
    auto err = esp_http_client_perform(client);

    if (err != ESP_OK) {
        ESP_LOGE(TAG, "HTTP GET request failed: %s", esp_err_to_name(err));
        return {};
    }

    JsonDocument filter;
    filter["departures"][0]["tripId"] = true;
    filter["departures"][0]["direction"] = true;
    filter["departures"][0]["line"]["name"] = true;
    filter["departures"][0]["when"] = true;

    JsonDocument doc;
    // TODO Extract stream creation to a function (easier said than done)
    std::unique_ptr<HTTPInputStream> inputStream(new HTTPInputStream(client));
    std::istream stream(inputStream.get());
    auto error = deserializeJson(doc, stream, DeserializationOption::Filter(filter));
    if (error) {
        ESP_LOGE(TAG, "Failed to parse JSON: %s", error.c_str());
        return {};
    }

    JsonArray departures = doc["departures"];
    auto departure_count = departures.size();
    ESP_LOGD(TAG, "Got %d departures", departure_count);

    std::vector<Trip> trips;
    trips.reserve(departure_count);

    Trip trip;
    for (auto departure : departures) {
        const char *tripId = departure["tripId"];
        const char *direction = departure["direction"];
        const char *line = departure["line"]["name"];

        const auto departure_time =
            departure["when"] == nullptr
                ? std::nullopt
                : std::make_optional(Time::iSO8601StringToTimePoint(static_cast<const char *>(departure["when"])));

        trips.push_back(
            {.tripId = tripId, .departureTime = departure_time, .directionName = direction, .lineName = line});
    }

    return trips;
}
