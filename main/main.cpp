#include <ArduinoJson.h>
#include <chrono>
#include <esp_http_client.h>
#include <esp_timer.h>
#include <esp_wifi.h>
#include <thread>

#include "lcd.h"
#include "ui_interaction.hpp"

using namespace std::chrono_literals;
using namespace std;

static void app_main_display(void) {
    ACQUIRE_LVGL_LOCK();
    ui_init();
    RELEASE_LVGL_LOCK();
}

extern "C" void app_main(void) {
    LVGL_LCD::init();

    app_main_display();

    while (true) {
        LogsScreen::addLogLine("Test " + to_string(esp_timer_get_time() / 1000));
        this_thread::sleep_for(100ms);
    }
}