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

    this_thread::sleep_for(2s);

    logs_screen.switchTo();
    logs_screen.addLogLine("It looks like you're trying to set up your device.");
    logs_screen.addLogLine("Please scan the following QR code to connect to the device:");
    logs_screen.addQRCode(PROVISIONING_QR_CODE_DATA);
    logs_screen.addLogLine("Alternatively, connect to the network \"" + AP_SSID + "\" with password \"" + AP_PASSWORD +
                           "\".");
    logs_screen.addLogLine("After connecting, please go to http://depmon.local to continue setup.");

    this_thread::sleep_for(5s);

    departures_screen.switchTo();
    for (int i = 0; i < 20; i++) {
        departures_screen.addRandomDepartureItem();
    }

    while (true) {
        this_thread::sleep_for(100ms);
    }
}