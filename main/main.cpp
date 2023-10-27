#include <ArduinoJson.h>
#include <chrono>
#include <esp_http_client.h>
#include <esp_mac.h>
#include <esp_timer.h>
#include <esp_wifi.h>
#include <mdns.h>
#include <nvs_flash.h>
#include <thread>

#include "lcd.h"
#include "ui.hpp"
#include "http_server.hpp"
#include "dns_server.h"

using namespace std::chrono_literals;
using namespace std;

// TODO The SSID shouldn't be constant, it should be different for each device
static const string AP_SSID = "esp32";
static const string AP_PASSWORD = "whatever";
static const string PROVISIONING_QR_CODE_DATA = "WIFI:S:" + AP_SSID + ";T:WPA;P:" + AP_PASSWORD + ";;";

static const char *WIFI_EVENT_HANDLER_TAG = "wifi_event_handler";

static void wifi_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data) {
    if (event_id == WIFI_EVENT_AP_STACONNECTED) {
        wifi_event_ap_staconnected_t *event = (wifi_event_ap_staconnected_t *)event_data;
        ESP_LOGI(WIFI_EVENT_HANDLER_TAG, "station " MACSTR " join, AID=%d", MAC2STR(event->mac), event->aid);
    } else if (event_id == WIFI_EVENT_AP_STADISCONNECTED) {
        wifi_event_ap_stadisconnected_t *event = (wifi_event_ap_stadisconnected_t *)event_data;
        ESP_LOGI(WIFI_EVENT_HANDLER_TAG, "station " MACSTR " leave, AID=%d", MAC2STR(event->mac), event->aid);
    }
}

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
    // TODO Maybe we need to use a public IP for the captive portal to work on Samsung devices? https://github.com/CDFER/Captive-Portal-ESP32/blob/main/src/main.cpp#L18
    ESP_ERROR_CHECK(esp_netif_init());

    // Create the default event loop
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    esp_netif_create_default_wifi_ap();
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler, NULL, NULL));

    wifi_config_t wifi_config = {
        .ap =
            {
                // TODO Make ssid and password dynamic
                .ssid = "esp32",
                .password = "whatever",
                .ssid_len = 0,
                .channel = 1,
                .authmode = WIFI_AUTH_WPA2_PSK,
                .max_connection = 1,
                .pmf_cfg =
                    {
                        .required = false,
                    },
            },
    };

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &wifi_config));

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
    // TODO Configure mDNS

    ESP_ERROR_CHECK(esp_wifi_start());

    setup_http_server();

    dns_server_config_t config = DNS_SERVER_CONFIG_SINGLE("*" /* all A queries */, "WIFI_AP_DEF" /* softAP netif ID */);
    start_dns_server(&config);

    // this_thread::sleep_for(5s);

    // departures_screen.switchTo();
    // for (int i = 0; i < 20; i++) {
    //     departures_screen.addRandomDepartureItem();
    // }
}