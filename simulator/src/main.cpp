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

    provisioning_screen.switchTo();
    provisioning_screen.addLine("It looks like you're trying to set up your device.");
    provisioning_screen.addLine(
        "Please download the \"ESP SoftAP Provisioning\" app from the App Store or Google Play, "
        "open it and follow the instructions.");
    provisioning_screen.addQRCode("LoremIpsumDolorSitAmet");
    this_thread::sleep_for(2s);

    provisioning_screen.addLine("Connected to WiFi! Switching to departures board...");

    departures_screen.switchTo();
    departures_screen.addTextItem("Station not found.");
    departures_screen.addTextItem("Please access http://suntransit.local/ to configure your station.");

    this_thread::sleep_for(2s);

    departures_screen.clean();
    for (int i = 0; i < 20; i++) {
        departures_screen.addRandomDepartureItem();
    }

    while (true) {
        this_thread::sleep_for(100ms);
    }
}
