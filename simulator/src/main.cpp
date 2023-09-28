#include <chrono>
#include <thread>
#include <string>
#include "lvgl_sdl.h"

using namespace std::chrono_literals;
using namespace std;

static void app_main_display(void) {
    ACQUIRE_LVGL_LOCK();
    ui_init();
    RELEASE_LVGL_LOCK();
}

int main(void) {
    LVGL_SDL::init();

    app_main_display();

    while (true) {
        LogsScreen::addLogLine("Hello world " + to_string(SDL_GetTicks()));
        this_thread::sleep_for(10ms);
    }
}