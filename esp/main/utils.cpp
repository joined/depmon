#include <esp_heap_caps_init.h>
#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <string>

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