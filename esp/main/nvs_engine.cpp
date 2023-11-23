#include <esp_log.h>

#include "nvs_engine.hpp"

static const char *TAG = "NVS";

// TODO Have a version number for the NVS namespace so that we can migrate data
// On opening the NVS, we should check the version number and erase the NVS if
// it's not the current version

NVSEngine::NVSEngine(const std::string nspace, nvs_open_mode mode) { nvs_open(nspace.c_str(), mode, &this->handle); };

NVSEngine::~NVSEngine() { nvs_close(this->handle); };

void NVSEngine::init() {
    esp_err_t err = nvs_flash_init();
    ESP_ERROR_CHECK(err);
    ESP_LOGD(TAG, "Initialized");
};

esp_err_t NVSEngine::readString(const std::string &key, std::string *result) {
    size_t len;
    auto err = nvs_get_str(this->handle, key.c_str(), nullptr, &len);
    if (err != ESP_OK) {
        return err;
    }

    char *data = (char *)malloc(len);
    nvs_get_str(this->handle, key.c_str(), data, &len);

    *result = std::string(data);
    free(data);

    return ESP_OK;
};

esp_err_t NVSEngine::readCurrentStation(JsonDocument *doc) {
    std::string currentStation;
    auto err = this->readString("current_station", &currentStation);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to read current station from NVS");
        return err;
    }

    JsonDocument currentStationIdFilter;
    currentStationIdFilter["id"] = true;

    auto deserializationError =
        deserializeJson(*doc, currentStation, DeserializationOption::Filter(currentStationIdFilter));
    if (deserializationError) {
        ESP_LOGE(TAG, "Failed to parse JSON: %s", deserializationError.c_str());
        return ESP_FAIL;
    }

    return ESP_OK;
};

esp_err_t NVSEngine::setString(const std::string &key, const std::string &value) {
    auto err = nvs_set_str(this->handle, key.c_str(), value.c_str());
    if (err) {
        return err;
    }
    err = nvs_commit(this->handle);
    return err;
};