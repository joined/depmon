#include <ArduinoJson.h>
#include <chrono>
#include <esp_http_client.h>
#include <esp_timer.h>
#include <esp_wifi.h>
#include <nvs_flash.h>
#include <thread>

#include "lcd.h"
#include "ui.hpp"

using namespace std::chrono_literals;
using namespace std;

// TODO The SSID shouldn't be constant, it should be different for each device
static const string AP_SSID = "esp32";
static const string AP_PASSWORD = "whatever";
static const string PROVISIONING_QR_CODE_DATA = "WIFI:S:" + AP_SSID + ";T:WPA;P:" + AP_PASSWORD + ";;";

extern "C" void app_main(void) {
    /* Initialize NVS partition */
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        /* NVS partition was truncated and needs to be erased */
        ESP_ERROR_CHECK(nvs_flash_erase());

        /* Retry nvs_flash_init */
        ESP_ERROR_CHECK(nvs_flash_init());
    }

    /* Initialize TCP/IP */
    ESP_ERROR_CHECK(esp_netif_init());

    // Create the default event loop
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    LVGL_LCD::init();

    UIManager::init();

    this_thread::sleep_for(2s);

    logs_screen.switchTo();
    logs_screen.addLogLine("It looks like you're trying to set up your device.");
    logs_screen.addLogLine("Please scan the following QR code to connect to the device:");
    logs_screen.addQRCode(PROVISIONING_QR_CODE_DATA);
    logs_screen.addLogLine("Alternatively, connect to the network \"" + AP_SSID + "\" with password \"" + AP_PASSWORD +
                           "\".");
    logs_screen.addLogLine("After connecting, please go to http://depmon.local to continue setup.");
    // TODO Make this work

    this_thread::sleep_for(5s);

    departures_screen.switchTo();
    for (int i = 0; i < 20; i++) {
        departures_screen.addRandomDepartureItem();
    }
}