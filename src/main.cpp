#include <Arduino.h>
#include <esp32_smartdisplay.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

#include <ui.h>

// TODO Is there a better way to do this?
#define STR1(x) #x
#define STR(x) STR1(x)

#define N_DEPARTURES 6
#define DEPARTURES_REFRESH_INTERVAL 2000
#define WIFI_CONNECT_TIMEOUT_MS 10000

lv_obj_t *departure_items[N_DEPARTURES];

const String stop_id = "900078102";
const String url = "https://v6.bvg.transport.rest/stops/" + stop_id + "/departures?results=7&duration=60&remarks=false";

void initMainscreen()
{
  // No need for a lock here because we're still in the setup function, no other task is running yet
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

void updateDepartureInfo(lv_obj_t *departure_item, const char *line_name, const char *direction, unsigned int diffmin)
{
  lv_obj_t *line = ui_comp_get_child(departure_item, UI_COMP_DEPARTUREITEM_DEPARTURE0LINE);
  lv_obj_t *destination = ui_comp_get_child(departure_item, UI_COMP_DEPARTUREITEM_DEPARTURE0DIRECTION);
  lv_obj_t *time_label = ui_comp_get_child(departure_item, UI_COMP_DEPARTUREITEM_DEPARTURE0TIME);

  char buffer[40];
  if (diffmin == 0)
  {
    sprintf(buffer, "", diffmin);
  }
  else
  {
    sprintf(buffer, "%u'", diffmin);
  }

  {
    const std::lock_guard<std::recursive_mutex> lock(lvgl_mutex);
    lv_label_set_text(line, line_name);
    lv_label_set_text(destination, direction);
    lv_label_set_text(time_label, buffer);
    lv_obj_clear_flag(departure_item, LV_OBJ_FLAG_HIDDEN);
  }
}

/* Takes about 250ms when reusing an existing connection */
bool performHttpRequest(HTTPClient &http, const char *url, DynamicJsonDocument &doc)
{
  http.begin(url); // This logs a warning because we're not verifying the certificate

  int httpResponseCode = http.GET();
  log_d("Response received");

  if (httpResponseCode <= 0)
  {
    log_d("Error on HTTP request");
    http.end();
    return false;
  }

  log_d("Parsing response");
  DeserializationError error = deserializeJson(doc, http.getStream());
  log_d("Response parsed");

  http.end(); // Close the HTTP connection

  if (error)
  {
    log_d("deserializeJson() failed: %s", error.c_str());
    return false;
  }

  return true;
}

/* Takes about 500ms */
void refreshDeparturesPanel(void *pvParameters)
{
  HTTPClient http;
  DynamicJsonDocument doc(12288);

  while (true)
  {
    log_d("Refreshing departures panel");

    // TODO Is there a better way to do this? Maybe we should stop/start the task when the WiFi connection changes?
    // Same for the time below
    if (!WiFi.isConnected())
    {
      log_d("WiFi is not connected, skipping refresh");
      vTaskDelay(pdMS_TO_TICKS(DEPARTURES_REFRESH_INTERVAL));
      continue;
    }

    if (!performHttpRequest(http, url.c_str(), doc))
    {
      log_d("Failed to refresh departures panel due to network error");
      vTaskDelay(pdMS_TO_TICKS(DEPARTURES_REFRESH_INTERVAL));
      continue;
    }

    JsonArray departures = doc["departures"];
    const unsigned int departure_count = departures.size();

    struct tm current_timeinfo;
    bool isTimeSet = getLocalTime(&current_timeinfo);

    if (!isTimeSet)
    {
      log_d("Failed to refresh departures panel due to time not being set");
      vTaskDelay(pdMS_TO_TICKS(DEPARTURES_REFRESH_INTERVAL));
      continue;
    }

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

      long diffsec = difftime(mktime(&timeinfo), mktime(&current_timeinfo));
      unsigned long diffsec_abs = diffsec < 0 ? 0 : diffsec;
      unsigned int diffmin = diffsec_abs / 60;

      updateDepartureInfo(departure_item, line_name, direction, diffmin);
    }

    if (departure_count < N_DEPARTURES)
    {
      const std::lock_guard<std::recursive_mutex> lock(lvgl_mutex);
      // hide last departures if we don't have enough to fill the screen
      for (int i = departure_count; i < N_DEPARTURES; i++)
      {
        lv_obj_add_flag(departure_items[i], LV_OBJ_FLAG_HIDDEN);
      }
    }
    log_d("Departures panel refreshed");

    http.end();

    vTaskDelay(pdMS_TO_TICKS(DEPARTURES_REFRESH_INTERVAL));
  }
}

void updateUi(void *pvParameters)
{
  while (true)
  {
    const std::lock_guard<std::recursive_mutex> lock(lvgl_mutex);
    lv_timer_handler();
  }
}

void ensureWifiConnected(void *pvParameters)
{
  while (true)
  {
    if (WiFi.status() == WL_CONNECTED)
    {
      log_d("WiFi is connected");
      vTaskDelay(pdMS_TO_TICKS(10000)); // If we're connected, check again in 10s
      continue;
    }

    log_d("WiFi is not connected, trying to connect");
    WiFi.mode(WIFI_STA);
    // TODO The reconnect might be handled by the WiFi library itself, so we might not need to do this
    WiFi.begin(STR(SECRET_WIFI_SSID), STR(SECRET_WIFI_PASSWORD));

    unsigned long startAttemptTime = millis();

    while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < WIFI_CONNECT_TIMEOUT_MS)
    {
      vTaskDelay(pdMS_TO_TICKS(500));
    }

    if (WiFi.status() == WL_CONNECTED)
    {
      smartdisplay_set_led_color(lv_color32_t({.ch = {.green = 40}}));
      log_d("Connected to WiFi network with IP Address: %s", WiFi.localIP().toString().c_str());
      vTaskDelay(pdMS_TO_TICKS(10000)); // We managed to connect, check again in 10s
    }
    else
    {
      smartdisplay_set_led_color(lv_color32_t({.ch = {.red = 40}}));
      log_d("Failed to connect to WiFi");
      vTaskDelay(pdMS_TO_TICKS(20000)); // If we're not connected, try again in 20s
    }
  }
}

void ensureTimeIsConfigured(void *pvParameters)
{
  struct tm timeinfo;

  while (true)
  {
    bool isTimeSet = getLocalTime(&timeinfo);

    // TODO Maybe re-configure time after some time so that it does not get out of sync (e.g. every 1 hour?)
    if (!isTimeSet)
    {
      log_d("No local time");

      if (WiFi.isConnected())
      {
        log_d("Configuring time");
        configTime(3600, 3600, "pool.ntp.org");
      }
      else
      {
        vTaskDelay(pdMS_TO_TICKS(5000)); // Try again in 5 seconds
      }
    }
    else
    {
      char buffer[40];
      strftime(buffer, sizeof(buffer), "%A, %B %d %Y %H:%M:%S", &timeinfo);
      log_d("Local time is configured. Current time: %s", buffer);

      // TODO We cannot have a `break` here or it will panic, why?
      vTaskDelay(pdMS_TO_TICKS(5000));
    }
  }
}

void setup()
{
  Serial.begin(115200);

  log_d("Initializing display");
  smartdisplay_init();
  log_d("Display initialized");

  smartdisplay_set_led_color(lv_color32_t({.ch = {.blue = 0, .green = 34, .red = 52}}));

  initMainscreen();

  xTaskCreatePinnedToCore(
      ensureWifiConnected,
      "ensureWifiConnected",
      8192, // TODO Is this enough? 8192 is used by `void loop` by default
      NULL,
      1,
      NULL,
      CONFIG_ARDUINO_RUNNING_CORE);

  xTaskCreatePinnedToCore(
      ensureTimeIsConfigured,
      "ensureTimeIsConfigured",
      8192, // TODO Is this enough? 8192 is used by `void loop` by default
      NULL,
      1,
      NULL,
      CONFIG_ARDUINO_RUNNING_CORE);

  xTaskCreatePinnedToCore(
      refreshDeparturesPanel,
      "refreshDeparturesPanel",
      8192, // TODO Is this enough? 8192 is used by `void loop` by default
      NULL,
      1,
      NULL,
      CONFIG_ARDUINO_RUNNING_CORE);

  xTaskCreatePinnedToCore(
      updateUi,
      "updateUi",
      8192, // TODO Is this enough? 8192 is used by `void loop` by default
      NULL,
      1,
      NULL,
      0);
}

void loop() {}