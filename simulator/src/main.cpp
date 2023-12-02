#include "lvgl_sdl.h"
#include <chrono>
#include <string>
#include <thread>

using namespace std::chrono_literals;
using namespace std;

static const string AP_SSID = "esp32";
static const string AP_PASSWORD = "whatever";
static const string PROVISIONING_QR_CODE_DATA = "WIFI:S:" + AP_SSID + ";T:WPA;P:" + AP_PASSWORD + ";;";

int main(void) {
    LVGL_SDL::init();

    UIManager::init();

    // this_thread::sleep_for(2s);

    // logs_screen.switchTo();
    // logs_screen.addLogLine("Lorem ipsum dolor sit amet, consectetur adipiscing elit. Donec a diam lectus. Sed sit amet ipsum mauris.");
    // logs_screen.addQRCode("LoremIpsumDolorSitAmet");
    // logs_screen.addLogLine("Lorem ipsum dolor sit amet, consectetur adipiscing elit. Donec a diam lectus. Sed sit amet ipsum mauris.");
    // logs_screen.addLogLine("Lorem ipsum dolor sit amet, consectetur adipiscing elit. Donec a diam lectus. Sed sit amet ipsum mauris.");
    // logs_screen.addLogLine("Lorem ipsum dolor sit amet, consectetur adipiscing elit. Donec a diam lectus. Sed sit amet ipsum mauris.");
    // logs_screen.addLogLine("Lorem ipsum dolor sit amet, consectetur adipiscing elit. Donec a diam lectus. Sed sit amet ipsum mauris.");
    // logs_screen.addLogLine("L o r e m i p s u m d o l o r s i t a m e t, c o n s e c t e t u r a d i p i s c i n g e l i t. D o n e c a d i a m l e c t u s. S e d s i t a m e t i p s u m m a u r i s.");

    // this_thread::sleep_for(5s);

    departures_screen.switchTo();
    for (int i = 0; i < 20; i++) {
        departures_screen.addRandomDepartureItem();
    }

    while (true) {
        this_thread::sleep_for(100ms);
    }
}
