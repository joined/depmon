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

extern "C" void app_main(void) {
    LVGL_LCD::init();

    {
        const std::lock_guard<std::recursive_mutex> lock(lvgl_mutex);
        ui_init();
    }

    this_thread::sleep_for(5s);

    // TODO This looks pretty bad, but it looks fine in the simulator. What's going on?
    LogsScreen::switchTo(LV_SCR_LOAD_ANIM_FADE_ON);
    this_thread::sleep_for(1s);

    // Ideas to test what constrains this loop:
    // Have a single label that is updated with the current time, so that less
    // of the screen is updated. Film with slow mo.
    for (int i = 0; i < 50; i++) {
        LogsScreen::addLogLine("Test " + to_string(esp_timer_get_time() / 1000));
        this_thread::sleep_for(100ms);
    }

    DeparturesScreen::switchTo(LV_SCR_LOAD_ANIM_OVER_LEFT);
    this_thread::sleep_for(1s);

    for (int i = 0; i < 20; i++) {
        DeparturesScreen::addRandomDepartureItem();
        this_thread::sleep_for(100ms);
    }
}