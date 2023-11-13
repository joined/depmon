#include <ArduinoJson.h>
#include <esp_app_desc.h>
#include <esp_http_server.h>
#include <esp_log.h>
#include <esp_random.h>
#include <esp_spiffs.h>
#include <esp_system.h>
#include <esp_vfs.h>
#include <fcntl.h>
#include <string>

#include "http_server.hpp"
#include "nvs_engine.hpp"

// 20KB scratch buffer for temporary storage during file transfer
#define SCRATCH_BUFSIZE (20480)

// Used when reading files from the filesystem
static char scratch[SCRATCH_BUFSIZE];

static const char *TAG = "http_server";

static esp_err_t init_fs(void) {
    esp_vfs_spiffs_conf_t conf = {
        .base_path = "/www", .partition_label = NULL, .max_files = 5, .format_if_mount_failed = false};
    esp_err_t ret = esp_vfs_spiffs_register(&conf);

    if (ret != ESP_OK) {
        if (ret == ESP_FAIL) {
            ESP_LOGE(TAG, "Failed to mount or format filesystem");
        } else if (ret == ESP_ERR_NOT_FOUND) {
            ESP_LOGE(TAG, "Failed to find SPIFFS partition");
        } else {
            ESP_LOGE(TAG, "Failed to initialize SPIFFS (%s)", esp_err_to_name(ret));
        }
        return ESP_FAIL;
    }

    size_t total = 0, used = 0;
    ret = esp_spiffs_info(NULL, &total, &used);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to get SPIFFS partition information (%s)", esp_err_to_name(ret));
    } else {
        ESP_LOGD(TAG, "Partition size: total: %d, used: %d", total, used);
    }
    return ESP_OK;
}

/* Set HTTP response content type according to file extension */
static esp_err_t set_content_type_from_file(httpd_req_t *req, const std::string &filepath) {
    const std::string cleaned_filepath =
        filepath.ends_with(".gz") ? filepath.substr(0, filepath.length() - 3) : filepath;
    const char *type = "text/plain";
    if (cleaned_filepath.ends_with(".html")) {
        type = "text/html";
    } else if (cleaned_filepath.ends_with(".js")) {
        type = "application/javascript";
    } else if (cleaned_filepath.ends_with(".css")) {
        type = "text/css";
    } else if (cleaned_filepath.ends_with(".png")) {
        type = "image/png";
    } else if (cleaned_filepath.ends_with(".ico")) {
        type = "image/x-icon";
    } else if (cleaned_filepath.ends_with(".svg")) {
        type = "text/xml";
    }
    return httpd_resp_set_type(req, type);
}

/* Send HTTP response with the contents of the requested file */
static esp_err_t rest_common_get_handler(httpd_req_t *req) {
    // TODO Make this more dynamic
    std::string filepath = "/www";
    const std::string uri = req->uri;
    if (uri.ends_with("/")) {
        filepath += "/index.html";
    } else {
        filepath += uri;
    }
    // Assumption: all files are gzipped. This won't be the case forever, probably.
    filepath += ".gz";

    ESP_LOGD(TAG, "Opening file %s", filepath.c_str());

    int file_descriptor = open(filepath.c_str(), O_RDONLY, 0);
    if (file_descriptor == -1) {
        ESP_LOGE(TAG, "Failed to open file : %s", filepath.c_str());
        /* Respond with 500 Internal Server Error */
        httpd_resp_send_err(req, HTTPD_404_NOT_FOUND, "File not found");
        return ESP_FAIL;
    }

    set_content_type_from_file(req, filepath);
    httpd_resp_set_hdr(req, "Content-Encoding", "gzip");

    ESP_LOGD(TAG, "Starting to send response for file: %s", filepath.c_str());

    char *chunk = scratch;
    ssize_t read_bytes;
    do {
        /* Read file in chunks into the scratch buffer */
        read_bytes = read(file_descriptor, chunk, SCRATCH_BUFSIZE);
        if (read_bytes == -1) {
            ESP_LOGE(TAG, "Failed to read file : %s", filepath.c_str());
        } else if (read_bytes > 0) {
            /* Send the buffer contents as HTTP response chunk */
            if (httpd_resp_send_chunk(req, chunk, read_bytes) != ESP_OK) {
                close(file_descriptor);
                ESP_LOGE(TAG, "File sending failed!");
                /* Abort sending file */
                httpd_resp_sendstr_chunk(req, NULL);
                /* Respond with 500 Internal Server Error */
                httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Failed to send file");
                return ESP_FAIL;
            }
        }
    } while (read_bytes > 0);
    /* Close file after sending complete */
    close(file_descriptor);
    ESP_LOGD(TAG, "File sending complete for file: %s", filepath.c_str());
    /* Respond with an empty chunk to signal HTTP response completion */
    httpd_resp_send_chunk(req, NULL, 0);
    return ESP_OK;
}

static esp_err_t api_get_version_handler(httpd_req_t *req) {
    httpd_resp_set_type(req, "application/json");
    JsonDocument doc;
    const esp_app_desc_t *app_description = esp_app_get_description();
    doc["version"] = app_description->version;
    doc["idf_version"] = app_description->idf_ver;
    doc["project_name"] = app_description->project_name;
    doc["compile_time"] = app_description->time;
    doc["compile_date"] = app_description->date;
    char buffer[64];
    serializeJson(doc, buffer);
    httpd_resp_sendstr(req, buffer);
    return ESP_OK;
}

static esp_err_t api_get_current_station_handler(httpd_req_t *req) {
    httpd_resp_set_type(req, "application/json");
    std::string current_station;
    NVSEngine nvs_engine("depmon");
    auto err = nvs_engine.readString("current_station", &current_station);
    JsonDocument doc;
    if (err != ESP_OK) {
        doc["id"] = nullptr;
    } else {
        doc["id"] = current_station;
    }
    char buffer[64];
    serializeJson(doc, buffer);
    httpd_resp_sendstr(req, buffer);
    return ESP_OK;
}

static esp_err_t api_set_current_station_handler(httpd_req_t *req) {
    char content[100];
    size_t recv_size = std::min(req->content_len, sizeof(content));
    int ret = httpd_req_recv(req, content, recv_size);
    if (ret <= 0) { /* 0 return value indicates connection closed */
        /* Check if timeout occurred */
        if (ret == HTTPD_SOCK_ERR_TIMEOUT) {
            /* In case of timeout one can choose to retry calling
             * httpd_req_recv(), but to keep it simple, here we
             * respond with an HTTP 408 (Request Timeout) error */
            httpd_resp_send_408(req);
        }
        /* In case of error, returning ESP_FAIL will
         * ensure that the underlying socket is closed */
        return ESP_FAIL;
    }
    JsonDocument doc;
    auto error = deserializeJson(doc, content);
    if (error) {
        httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, "Invalid JSON");
        return ESP_FAIL;
    }

    const std::string id = doc["id"];
    NVSEngine nvs_engine("depmon");
    auto err = nvs_engine.setString("current_station", id);
    if (err != ESP_OK) {
        httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Failed to set current station");
        return ESP_FAIL;
    }

    httpd_resp_set_type(req, "application/json");
    httpd_resp_sendstr(req, "{}");
    return ESP_OK;
}

httpd_handle_t setup_http_server() {
    init_fs();
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.uri_match_fn = httpd_uri_match_wildcard;
    httpd_handle_t server = NULL;

    ESP_ERROR_CHECK(httpd_start(&server, &config));

    httpd_uri_t api_get_version_uri = {
        .uri = "/api/version",
        .method = HTTP_GET,
        .handler = api_get_version_handler,
    };
    httpd_register_uri_handler(server, &api_get_version_uri);

    httpd_uri_t api_get_current_station_uri = {
        .uri = "/api/currentstation",
        .method = HTTP_GET,
        .handler = api_get_current_station_handler,
    };
    httpd_register_uri_handler(server, &api_get_current_station_uri);

    httpd_uri_t api_set_current_station_uri = {
        .uri = "/api/currentstation",
        .method = HTTP_POST,
        .handler = api_set_current_station_handler,
    };
    httpd_register_uri_handler(server, &api_set_current_station_uri);

    /* URI handler for getting web server files */
    httpd_uri_t common_get_uri = {.uri = "/*", .method = HTTP_GET, .handler = rest_common_get_handler};
    httpd_register_uri_handler(server, &common_get_uri);

    return server;
}