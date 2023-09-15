#include <Arduino.h>
#include <esp32_smartdisplay.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

#include <ui.h>

// TODO Is there a better way to do this?
#define STR1(x)  #x
#define STR(x)  STR1(x)

#define N_DEPARTURES 6
#define UPDATE_INTERVAL 2000

lv_obj_t *departure_items[N_DEPARTURES];

const String url = "https://v6.bvg.transport.rest/stops/900078102/departures?results=7&duration=60";

void initMainscreen()
{
  const std::lock_guard<std::recursive_mutex> lock(lvgl_mutex);

  log_d("Cleaning screen");
  lv_obj_clean(lv_scr_act());
  log_d("Screen cleaned");

  log_d("Loading UI");
  ui_init();
  log_d("UI loaded");

  departure_items[0] = ui_departureitem1;
  departure_items[1] = ui_departureitem2;
  departure_items[2] = ui_departureitem3;
  departure_items[3] = ui_departureitem4;
  departure_items[4] = ui_departureitem5;
  departure_items[5] = ui_departureitem6;
}

void updateDepartureInfo(lv_obj_t *departure_item, const char *line_name, const char *direction, unsigned int diffmin) {
    lv_obj_t *line = ui_comp_get_child(departure_item, UI_COMP_DEPARTUREITEM_DEPARTURE0LINE);
    lv_obj_t *destination = ui_comp_get_child(departure_item, UI_COMP_DEPARTUREITEM_DEPARTURE0DIRECTION);
    lv_obj_t *time_label = ui_comp_get_child(departure_item, UI_COMP_DEPARTUREITEM_DEPARTURE0TIME);

    if (line == NULL || destination == NULL || time_label == NULL) {
        // Should never happen, do we need to handle this?
        return;
    }

    char buffer[40];
    if (diffmin == 0) {
        sprintf(buffer, "", diffmin);
    } else {
        sprintf(buffer, "%u'", diffmin);
    }

    lv_label_set_text(line, line_name);
    lv_label_set_text(destination, direction);
    lv_label_set_text(time_label, buffer);
}

void refreshDeparturesPanel(lv_timer_t *timer)
{
  HTTPClient http;
  http.begin(url); // This logs a warning because we're not verifying the certificate

  int httpResponseCode = http.GET();
  if (httpResponseCode > 0)
  {
    DynamicJsonDocument doc(12288);
    DeserializationError error = deserializeJson(doc, http.getStream());

    if (error)
    {
      log_d("deserializeJson() failed: %s", error.c_str());
      return;
    }

    JsonArray departures = doc["departures"];
    const unsigned int departure_count = departures.size();

    for (int i = 0; i < N_DEPARTURES; i++)
    {
      lv_obj_t *departure_item = departure_items[i];

      JsonObject departure = departures[i];
      const char *direction = departure["direction"];    // e.g. "U Rudow"
      const char *when = departure["when"];              // e.g. "2020-11-22T15:51:00+01:00"
      const char *line_name = departure["line"]["name"]; // e.g. "U7"

      struct tm timeinfo = {0};

      // TODO This is weird. Why do we need to set tm_isdst to 1?
      strptime(when, "%FT%T%z", &timeinfo);
      timeinfo.tm_isdst = 1;

      struct tm current_timeinfo;
      getLocalTime(&current_timeinfo); // TODO Handle error in getLocalTime?

      long diffsec = difftime(mktime(&timeinfo), mktime(&current_timeinfo));
      unsigned long diffsec_abs = diffsec < 0 ? 0 : diffsec;
      unsigned int diffmin = diffsec_abs / 60;

      updateDepartureInfo(departure_item, line_name, direction, diffmin);

      lv_obj_clear_flag(departure_items[i], LV_OBJ_FLAG_HIDDEN);
    }

    // hide last departures if we don't have enough to fill the screen
    for (int i = departure_count; i < N_DEPARTURES; i++)
    {
      lv_obj_add_flag(departure_items[i], LV_OBJ_FLAG_HIDDEN);
    }
  }
}

void connectToWifi()
{
  log_d("Connecting to WiFi");
  WiFi.begin(STR(SECRET_WIFI_SSID), STR(SECRET_WIFI_PASSWORD));

  unsigned int wifi_connect_tries = 0;
  while (WiFi.status() != WL_CONNECTED)
  {
    if (wifi_connect_tries++ > 20)
    {
      log_d("Failed to connect to WiFi");
      return;
    }
    delay(500);
  }

  log_d("Connected to WiFi network with IP Address: %s", WiFi.localIP().toString().c_str());
}

void configureTime()
{
  log_d("Setting time servers");

  const int gmtOffset_sec = 3600;
  const int daylightOffset_sec = 3600;

  configTime(gmtOffset_sec, daylightOffset_sec, "pool.ntp.org");

  struct tm timeinfo;
  getLocalTime(&timeinfo);
  if (!getLocalTime(&timeinfo))
  {
    log_d("Failed to obtain time");
    return;
  }

  char buffer[40];
  strftime(buffer, sizeof(buffer), "%A, %B %d %Y %H:%M:%S", &timeinfo);
  log_d("Current time: %s", buffer);
}

void setup()
{
  Serial.begin(115200);

  log_d("Initializing display");
  smartdisplay_init();
  log_d("Display initialized");

  initMainscreen();

  connectToWifi();

  smartdisplay_set_led_color(WiFi.isConnected() ? lv_color32_t({.ch = {.green = 40}}) : lv_color32_t({.ch = {.red = 40}}));

  configureTime();

  lv_timer_t *timer = lv_timer_create(refreshDeparturesPanel, UPDATE_INTERVAL, NULL);
}

void loop()
{
  lv_timer_handler();
}