#include <esp_heap_caps_init.h>
#include <esp_log.h>
#include <esp_mac.h>
#include <esp_wifi.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include "utils.hpp"

static const char *TAG = "Utils";

void printHealthStats(const std::string &tag) {
    ESP_LOGI(TAG, "Health status count tag: %s", tag.c_str());
    ESP_LOGI(TAG, "Current stack high water mark: %d", uxTaskGetStackHighWaterMark(NULL));
    ESP_LOGI(TAG, "Current free heap: %d", static_cast<int>(heap_caps_get_free_size(MALLOC_CAP_DEFAULT)));
    ESP_LOGI(TAG, "Current free heap largest block: %d",
             static_cast<int>(heap_caps_get_largest_free_block(MALLOC_CAP_DEFAULT)));
    ESP_LOGI(TAG, "Alltime minimum free heap: %d",
             static_cast<int>(heap_caps_get_minimum_free_size(MALLOC_CAP_DEFAULT)));
}

/* Will have format `1A:2B:3C:4D:5E:6F` if separated, `1A2B3C4D5E6F` if not */
std::string getMacString(const bool &separated) {
    uint8_t mac[6];
    esp_efuse_mac_get_default(mac);

    char mac_string[18];
    snprintf(mac_string, sizeof(mac_string), separated ? "%02X:%02X:%02X:%02X:%02X:%02X" : "%02X%02X%02X%02X%02X%02X",
             mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    return std::string(mac_string);
}

/* Will have format `PREFIX2A3` */
std::string getUniqueTag(const std::string &prefix) {
    // TODO Are MACs random enough? Should we pass this through a hash function?
    const auto mac = getMacString(false);
    return prefix + mac.substr(0, 3);
}