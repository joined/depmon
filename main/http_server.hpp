#include <esp_http_server.h>
#include <esp_log.h>

extern const char rootHtml[] asm("_binary_root_html_start");

esp_err_t root_req_handler(httpd_req_t *req) { 
    ESP_LOGI("HTTPD", "GET request received");
    httpd_resp_set_type(req, "text/html");
    return httpd_resp_send(req, rootHtml, HTTPD_RESP_USE_STRLEN);
}

httpd_uri_t root = {.uri = "/", .method = HTTP_GET, .handler = root_req_handler, .user_ctx = NULL};

httpd_handle_t setup_http_server(void) {
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    httpd_handle_t server = NULL;

    if (httpd_start(&server, &config) == ESP_OK) {
        httpd_register_uri_handler(server, &root);
    }

    return server;
}