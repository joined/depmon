#include <ArduinoJson.h>
#include <chrono>
#include <ctime>
#include <esp_http_client.h>
#include <esp_log.h>
#include <string>
#include <vector>

#include "bvg_api_client.hpp"
#include "time.hpp"

static const char *TAG = "BvgApiClient";

BvgApiClient::BvgApiClient(const std::string &stationId) {
    std::string url =
        "https://v6.bvg.transport.rest/stops/" + stationId + "/departures?results=" + std::to_string(N_RESULTS);

    esp_http_client_config_t config = {
        .url = url.c_str(),
        .event_handler =
            [](esp_http_client_event_t *evt) {
                auto self = static_cast<BvgApiClient *>(evt->user_data);
                return self->http_event_handler(evt);
            },
        .user_data = this,
    };
    client = esp_http_client_init(&config);
    esp_http_client_set_method(client, HTTP_METHOD_GET);
    esp_http_client_set_header(client, "Accept", "application/json");
}

BvgApiClient::~BvgApiClient() { esp_http_client_cleanup(client); }

esp_err_t BvgApiClient::http_event_handler(esp_http_client_event_t *evt) {
    switch (evt->event_id) {
    case HTTP_EVENT_ON_DATA:
        ESP_LOGD(TAG, "HTTP_EVENT_ON_DATA, data_len=%d", evt->data_len);
        ESP_LOGD(TAG, "Current buffer size: %d", this->current_buffer_size);
        ESP_LOGD(TAG, "Current buffer_pos: %d", buffer_pos);

        if (this->output_buffer == NULL) {
            this->current_buffer_size = START_BUFFER_SIZE;
            this->output_buffer = (char *)malloc(this->current_buffer_size);
            this->buffer_pos = 0;
            if (this->output_buffer == NULL) {
                ESP_LOGE(TAG, "Failed to allocate memory for output buffer");
                return ESP_FAIL;
            }
        } else {
            while (this->buffer_pos + evt->data_len >= this->current_buffer_size) {
                ESP_LOGD(TAG, "Would overflow buffer, reallocating");
                this->current_buffer_size *= this->BUFFER_GROWTH_FACTOR;
                this->output_buffer = (char *)realloc(this->output_buffer, this->current_buffer_size);

                if (this->output_buffer == NULL) {
                    ESP_LOGE(TAG, "Failed to reallocate memory for output buffer");
                    return ESP_FAIL;
                }
            }
        }
        memcpy(this->output_buffer + buffer_pos, evt->data, evt->data_len);
        this->buffer_pos += evt->data_len;
        break;

    case HTTP_EVENT_ON_FINISH:
        ESP_LOGD(TAG, "HTTP_EVENT_ON_FINISH");
        this->buffer_pos = 0;
        break;
    // TODO Figure out when this happens and if we're handling it correctly
    case HTTP_EVENT_DISCONNECTED:
        ESP_LOGE(TAG, "HTTP_EVENT_DISCONNECTED");
        free(this->output_buffer);
        this->output_buffer = NULL;
        buffer_pos = 0;
        break;
    default:
        break;
    }

    return ESP_OK;
}

std::vector<Trip> BvgApiClient::fetchAndParseTrips() {
    esp_http_client_perform(client);

    if (this->output_buffer == NULL) {
        ESP_LOGE(TAG, "No response buffer, something went wrong");
        return {};
    }

    StaticJsonDocument<200> filter;
    filter["departures"][0]["tripId"] = true;
    filter["departures"][0]["direction"] = true;
    filter["departures"][0]["line"]["name"] = true;
    filter["departures"][0]["when"] = true;

    DynamicJsonDocument doc(this->MAX_JSON_DOC_SIZE);
    // TODO It would be cool to use a std::istream here, would probably save memory too.
    DeserializationError error = deserializeJson(doc, this->output_buffer, DeserializationOption::Filter(filter));
    if (error) {
        ESP_LOGE(TAG, "Failed to parse JSON: %s", error.c_str());
        return {};
    }

    JsonArray departures = doc["departures"];
    auto departure_count = departures.size();
    ESP_LOGI(TAG, "Got %d departures", departure_count);

    std::vector<Trip> trips;
    trips.reserve(departure_count);

    for (auto departure : departures) {
        const char *tripId = departure["tripId"];
        const char *direction = departure["direction"];
        const char *line = departure["line"]["name"];
        const char *when = departure["when"];

        trips.push_back({.tripId = tripId,
                         .departureTime = Time::iSO8601StringToTimePoint(when),
                         .directionName = direction,
                         .lineName = line});
    }

    free(this->output_buffer);
    this->output_buffer = NULL;

    return trips;
}