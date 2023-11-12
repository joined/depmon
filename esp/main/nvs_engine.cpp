#include <esp_log.h>

#include "nvs_engine.hpp"

static const char *TAG = "NVS";

NVSEngine::NVSEngine(const std::string nspace, nvs_open_mode mode) {
    nvs_open(nspace.c_str(), mode, &this->handle);
};

NVSEngine::~NVSEngine() {
    nvs_close(this->handle);
};

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

esp_err_t NVSEngine::setString(const std::string &key, const std::string &value) {
    return nvs_set_str(this->handle, key.c_str(), value.c_str());
};