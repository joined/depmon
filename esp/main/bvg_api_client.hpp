#pragma once

#include <ctime>
#include <esp_http_client.h>
#include <string>
#include <vector>

struct Trip {
    std::string tripId;
    std::chrono::system_clock::time_point departureTime;
    std::string directionName;
    std::string lineName;
};

class BvgApiClient {
  public:
    BvgApiClient(const std::string &stationId);
    ~BvgApiClient();
    std::vector<Trip> fetchAndParseTrips();

  private:
    esp_http_client_handle_t client;
    esp_err_t http_event_handler(esp_http_client_event_t *evt);
    char *output_buffer = NULL;
    int buffer_pos;

    static constexpr const int START_BUFFER_SIZE = 10 * 1024;
    static constexpr const int MAX_JSON_DOC_SIZE = 16 * 1024;
    static constexpr const double BUFFER_GROWTH_FACTOR = 1.5;
    static constexpr const int N_RESULTS = 10;
    size_t current_buffer_size;
};