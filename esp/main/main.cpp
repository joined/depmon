#include <chrono>
#include <esp_http_client.h>
#include <esp_log.h>
#include <esp_mac.h>
#include <esp_timer.h>
#include <esp_wifi.h>
#include <freertos/event_groups.h>
#include <lwip/apps/netbiosns.h>
#include <mdns.h>
#include <nvs_flash.h>
#include <sys/param.h>
#include <thread>
#include <wifi_provisioning/manager.h>
#include <wifi_provisioning/scheme_softap.h>

#include "bvg_api_client.hpp"
#include "http_server.hpp"
#include "lcd.hpp"
#include "time.hpp"
#include "ui.hpp"
#include "utils.hpp"

/* Signal Wi-Fi events on this event-group */
const int WIFI_CONNECTED_EVENT = BIT0;
static EventGroupHandle_t wifi_event_group;

static const char *TAG = "MAIN";

#define PROV_MGR_MAX_RETRY_COUNT 3

using namespace std::chrono_literals;

static void provisioning_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data) {
    static int retries;
    if (event_base == WIFI_PROV_EVENT) {
        switch (event_id) {
        case WIFI_PROV_START:
            ESP_LOGI(TAG, "Provisioning started");
            break;
        case WIFI_PROV_CRED_RECV: {
            wifi_sta_config_t *wifi_sta_cfg = (wifi_sta_config_t *)event_data;
            ESP_LOGI(TAG,
                     "Received Wi-Fi credentials"
                     "\n\tSSID     : %s\n\tPassword : %s",
                     (const char *)wifi_sta_cfg->ssid, (const char *)wifi_sta_cfg->password);
            break;
        }
        case WIFI_PROV_CRED_FAIL: {
            wifi_prov_sta_fail_reason_t *reason = (wifi_prov_sta_fail_reason_t *)event_data;
            ESP_LOGE(TAG,
                     "Provisioning failed!\n\tReason : %s"
                     "\n\tPlease reset to factory and retry provisioning",
                     (*reason == WIFI_PROV_STA_AUTH_ERROR) ? "Wi-Fi station authentication failed"
                                                           : "Wi-Fi access-point not found");
            retries++;
            if (retries >= PROV_MGR_MAX_RETRY_COUNT) {
                ESP_LOGI(TAG, "Failed to connect with provisioned AP, reseting provisioned credentials");
                wifi_prov_mgr_reset_sm_state_on_failure();
                retries = 0;
            }
            break;
        }
        case WIFI_PROV_CRED_SUCCESS:
            ESP_LOGI(TAG, "Provisioning successful");
            retries = 0;
            break;
        case WIFI_PROV_END:
            /* De-initialize manager once provisioning is finished */
            wifi_prov_mgr_deinit();
            break;
        default:
            break;
        }
    } else if (event_base == WIFI_EVENT) {
        switch (event_id) {
        case WIFI_EVENT_STA_START:
            esp_wifi_connect();
            break;
        case WIFI_EVENT_STA_DISCONNECTED:
            ESP_LOGI(TAG, "Disconnected. Connecting to the AP again...");
            esp_wifi_connect();
            break;
        case WIFI_EVENT_AP_STACONNECTED:
            ESP_LOGI(TAG, "SoftAP transport: Connected!");
            break;
        case WIFI_EVENT_AP_STADISCONNECTED:
            ESP_LOGI(TAG, "SoftAP transport: Disconnected!");
            break;
        default:
            break;
        }
    } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        ip_event_got_ip_t *event = (ip_event_got_ip_t *)event_data;
        ESP_LOGI(TAG, "Connected with IP Address:" IPSTR, IP2STR(&event->ip_info.ip));
        /* Signal main application to continue execution */
        xEventGroupSetBits(wifi_event_group, WIFI_CONNECTED_EVENT);
    }
}

static void init_mdns_and_netbios(void) {
    ESP_ERROR_CHECK(mdns_init());
    ESP_ERROR_CHECK(mdns_hostname_set("depmon"));
    ESP_ERROR_CHECK(mdns_instance_name_set("DepMon"));

    mdns_txt_item_t serviceTxtData[] = {{"board", "esp32"}, {"path", "/"}};
    ESP_ERROR_CHECK(mdns_service_add("DepMon Configuration Server", "_http", "_tcp", 80, serviceTxtData,
                                     sizeof(serviceTxtData) / sizeof(serviceTxtData[0])));

    netbiosns_init();
    netbiosns_set_name("depmon");
}

static void init_nvs() {
    /* Initialize NVS partition */
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        /* NVS partition was truncated and needs to be erased */
        ESP_ERROR_CHECK(nvs_flash_erase());
        /* Retry nvs_flash_init */
        ESP_ERROR_CHECK(nvs_flash_init());
    }
}

/** Will have format `PROV_2A3B4C` */
std::string getProvisioningSSID() {
    uint8_t eth_mac[6];
    esp_wifi_get_mac(WIFI_IF_STA, eth_mac);

    const std::string ssid_prefix = "PROV_";
    char service_name[12];
    snprintf(service_name, sizeof(service_name), "%s%02X%02X%02X", ssid_prefix.c_str(), eth_mac[3], eth_mac[4],
             eth_mac[5]);

    return std::string(service_name);
};

static void wifi_prov_print_qr(const std::string &name) {
    const std::string payload = "{\"ver\":\"v1\",\"name\":\"" + name + "\",\"transport\":\"softap\"}";
    logs_screen.addQRCode(payload);
}

void init_network_wifi_and_wifimanager() {
    /* Initialize TCP/IP */
    ESP_ERROR_CHECK(esp_netif_init());

    init_mdns_and_netbios();

    wifi_event_group = xEventGroupCreate();

    /* Register our event handlers for Wi-Fi/IP related events */
    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_PROV_EVENT, ESP_EVENT_ANY_ID, &provisioning_event_handler, NULL));
    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &provisioning_event_handler, NULL));
    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &provisioning_event_handler, NULL));

    /* Initialize Wi-Fi including netif with default config */
    esp_netif_create_default_wifi_sta();
    esp_netif_create_default_wifi_ap();
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    /* Configuration for the provisioning manager */
    wifi_prov_mgr_config_t config = {.scheme = wifi_prov_scheme_softap,
                                     .scheme_event_handler = WIFI_PROV_EVENT_HANDLER_NONE};

    /* Initialize provisioning manager with the configuration parameters set above */
    ESP_ERROR_CHECK(wifi_prov_mgr_init(config));
}

QueueHandle_t departuresRefreshQueue = xQueueCreate(1, sizeof(uint8_t));

void fetch_and_process_trips(BvgApiClient &apiClient) {
    auto trips = apiClient.fetchAndParseTrips();

    if (trips.empty()) {
        ESP_LOGE(TAG, "No trips found!");
        return;
    }

    ESP_LOGI(TAG, "Found %d trips", trips.size());

    departures_screen.clean();
    const auto now = Time::timePointNow();
    for (auto trip : trips) {
        const auto timeToDeparture = trip.departureTime - now;
        departures_screen.addItem(trip.lineName, trip.directionName,
                                  std::chrono::duration_cast<std::chrono::seconds>(timeToDeparture));
    }
}

void DeparturesRefresherTask(void *pvParameter) {
    uint8_t message;

    auto apiClient = BvgApiClient("900078102");

    while (true) {
        if (xQueueReceive(departuresRefreshQueue, &message, 0) == pdPASS) {
            printHealthStats("DeparturesRefresherTask");

            fetch_and_process_trips(apiClient);
        }
        std::this_thread::sleep_for(10ms);
    }

    vTaskDelete(NULL);
}

const esp_timer_create_args_t departuresRefresherTimerArgs = {
    .callback =
        [](void *arg) {
            uint8_t message = 1;
            xQueueSend(departuresRefreshQueue, &message, 0);
        },
    .name = "departuresRefreshTimer",
};

esp_timer_handle_t departuresRefreshTimerHandle = nullptr;

extern "C" void app_main(void) {
    printHealthStats("app_main start");
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    LVGL_LCD::init();
    UIManager::init();

    init_nvs();
    init_network_wifi_and_wifimanager();

    // 3356 (words, = 13424 bytes) is the max observed used stack
    xTaskCreatePinnedToCore(DeparturesRefresherTask, "DeparturesRefresherTask", 4096, NULL, 1, NULL, 0);

    bool provisioned = false;
    /* Let's find out if the device is provisioned */
    ESP_ERROR_CHECK(wifi_prov_mgr_is_provisioned(&provisioned));

    if (!provisioned) {
        ESP_LOGI(TAG, "Starting provisioning");

        std::string service_name = getProvisioningSSID();
        ESP_ERROR_CHECK(wifi_prov_mgr_start_provisioning(WIFI_PROV_SECURITY_0, nullptr, service_name.c_str(), nullptr));
        logs_screen.switchTo();
        logs_screen.addLogLine("It looks like you're trying to set up your device.");
        logs_screen.addLogLine("Please download the ESP SoftAP Provisioning app from the App Store or Google Play, "
                               "open it and follow the instructions.");
        wifi_prov_print_qr(service_name.c_str());
    } else {
        ESP_LOGI(TAG, "Already provisioned, starting Wi-Fi STA");
        splash_screen.updateStatus("Connecting to WiFi...");

        /* We don't need the manager as device is already provisioned, so let's release it's resources */
        wifi_prov_mgr_deinit();

        /* Start Wi-Fi in station mode */
        ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
        ESP_ERROR_CHECK(esp_wifi_start());
    }

    /* Wait for Wi-Fi connection */
    xEventGroupWaitBits(wifi_event_group, WIFI_CONNECTED_EVENT, true, true, portMAX_DELAY);

    if (provisioned) {
        splash_screen.updateStatus("Connected! Switching to departures screen...");
    } else {
        logs_screen.addLogLine("Connected to WiFi! Switching to departures board...");
    }

    std::this_thread::sleep_for(2s);

    setup_http_server();

    Time::initSNTP();

    departures_screen.switchTo();
    ESP_ERROR_CHECK(esp_timer_create(&departuresRefresherTimerArgs, &departuresRefreshTimerHandle));
    ESP_ERROR_CHECK(
        esp_timer_start_periodic(departuresRefreshTimerHandle, duration_cast<std::chrono::microseconds>(5s).count()));

    printHealthStats("end of app_main");
};