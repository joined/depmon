#include <esp_log.h>
#include <esp_sntp.h>
#include <sys/time.h>
#include <time.h>

#include "time.hpp"

static const char *TAG = "Time";
static auto synced = false;

namespace Time {
void callbackOnNtpUpdate(timeval *tv) {
    ESP_LOGI(TAG, "NTP updated, current time is: %s", timeNowAscii().c_str());
    synced = true;
};

esp_err_t initSNTP() {
    if (!synced) {
        setenv("TZ", "CET-1CEST,M3.5.0,M10.5.0/3", 1);
        tzset();

        esp_sntp_setoperatingmode(SNTP_OPMODE_POLL);
        esp_sntp_setservername(0, "time.google.com");
        esp_sntp_setservername(1, "pool.ntp.com");

        sntp_set_time_sync_notification_cb(&callbackOnNtpUpdate);
        sntp_set_sync_interval(60 * 60 * 1000);

        esp_sntp_init();

        ESP_LOGD(TAG, "SNTP Initialized");
    }

    if (synced) {
        return ESP_OK;
    }
    return ESP_FAIL;
}

const std::chrono::system_clock::time_point timePointNow() { return std::chrono::system_clock::now(); }
std::string timeNowAscii() {
    const std::time_t timeNow{std::chrono::system_clock::to_time_t(timePointNow())};

    return std::asctime(std::localtime(&timeNow));
}

const std::chrono::time_point<std::chrono::system_clock, std::chrono::seconds>
iSO8601StringToTimePoint(const std::string &iso8601) {
    std::tm t = {};
    // F: Equivalent to %Y-%m-%d, the ISO 8601 date format.
    // T: ISO 8601 time format (HH:MM:SS), equivalent to %H:%M:%S
    // z: ISO 8601 offset from UTC in timezone (1 minute=1, 1 hour=100). If timezone cannot be determined, no characters
    strptime(iso8601.c_str(), "%FT%T%z", &t);
    return std::chrono::time_point<std::chrono::system_clock, std::chrono::seconds>(std::chrono::seconds(mktime(&t)));
}
} // namespace Time