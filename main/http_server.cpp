#include <esp_http_server.h>
#include <esp_log.h>
#include <esp_vfs.h>
#include <fcntl.h>
#include <string>

#include "http_server.hpp"

// 20KB scratch buffer for temporary storage during file transfer
#define SCRATCH_BUFSIZE (20480)

// Used when reading files from the filesystem
static char scratch[SCRATCH_BUFSIZE];

static const char *TAG = "http_server";

// TODO We should probably switch to async request handling, but it's only available
// in the `master` branch of ESP-IDF at the moment:
// https://github.com/espressif/esp-idf/tree/master/examples/protocols/http_server/async_handlers
// If we do switch, we need different scratch buffers for each request, I guess.

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

    ESP_LOGI(TAG, "Opening file %s", filepath.c_str());

    int file_descriptor = open(filepath.c_str(), O_RDONLY, 0);
    if (file_descriptor == -1) {
        ESP_LOGE(TAG, "Failed to open file : %s", filepath.c_str());
        /* Respond with 500 Internal Server Error */
        httpd_resp_send_err(req, HTTPD_404_NOT_FOUND, "File not found");
        return ESP_FAIL;
    }

    set_content_type_from_file(req, filepath);
    httpd_resp_set_hdr(req, "Content-Encoding", "gzip");

    ESP_LOGI(TAG, "Starting to send response for file: %s", filepath.c_str());

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
    ESP_LOGI(TAG, "File sending complete for file: %s", filepath.c_str());
    /* Respond with an empty chunk to signal HTTP response completion */
    httpd_resp_send_chunk(req, NULL, 0);
    return ESP_OK;
}

httpd_handle_t setup_http_server() {
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.uri_match_fn = httpd_uri_match_wildcard;
    httpd_handle_t server = NULL;

    ESP_ERROR_CHECK(httpd_start(&server, &config));

    /* URI handler for getting web server files */
    httpd_uri_t common_get_uri = {
        .uri = "/*", .method = HTTP_GET, .handler = rest_common_get_handler};
    httpd_register_uri_handler(server, &common_get_uri);

    return server;
}