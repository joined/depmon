#pragma once

#include <ArduinoJson.h>
#include <ctime>
#include <esp_http_client.h>
#include <optional>
#include <string>
#include <vector>

struct Trip {
    const std::string tripId;
    const std::optional<std::chrono::system_clock::time_point> departureTime;
    const std::string directionName;
    const std::string lineName;
};

class BvgApiClient {
  public:
    BvgApiClient(const std::string &stationId);
    ~BvgApiClient();
    std::vector<Trip> fetchAndParseTrips();

  private:
    esp_http_client_handle_t client;
    esp_err_t http_event_handler(esp_http_client_event_t *evt);
    void setUrl(const std::string &stationId, const JsonArray &enabledProducts);
    char *output_buffer = NULL;
    int buffer_pos;

    static constexpr const int HTTP_BUFFER_START_SIZE = 15 * 1024;
    // TODO Maybe drop the buffer growth factor and just allocate a large buffer
    static constexpr const double BUFFER_GROWTH_FACTOR = 1.5;
    static constexpr const int N_RESULTS = 12;
    size_t current_buffer_size;
};