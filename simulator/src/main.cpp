#include <chrono>
#include <thread>
#include <string>
#include "lvgl_sdl.h"

using namespace std::chrono_literals;
using namespace std;

int main(void) {
    LVGL_SDL::init();

    {
        const std::lock_guard<std::recursive_mutex> lock(lvgl_mutex);
        ui_init();
    }

    this_thread::sleep_for(5s);

    LogsScreen::switchTo();

    // Ideas to test what constrains this loop:
    // Have a single label that is updated with the current time, so that less
    // of the screen is updated. Film with slow mo.
    for (int i = 0; i < 50; i++) {
        LogsScreen::addLogLine("Test " + to_string(SDL_GetTicks()));
        this_thread::sleep_for(100ms);
    }

    DeparturesScreen::switchTo();

    for (int i = 0; i < 20; i++) {
        DeparturesScreen::addRandomDepartureItem();
        this_thread::sleep_for(100ms);
    }

    while (true) {
        this_thread::sleep_for(100ms);
    }
}