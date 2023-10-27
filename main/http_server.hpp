#include <esp_http_server.h>
#include <esp_log.h>

static const char *HTTP_SERVER_TAG = "HTTP Server";
extern const char rootHtml[] asm("_binary_root_html_start");

esp_err_t root_req_handler(httpd_req_t *req) { 
    ESP_LOGI("HTTPD", "GET request received");
    httpd_resp_set_type(req, "text/html");
    return httpd_resp_send(req, rootHtml, HTTPD_RESP_USE_STRLEN);
}

httpd_uri_t root = {.uri = "/", .method = HTTP_GET, .handler = root_req_handler, .user_ctx = NULL};

esp_err_t http_404_error_handler(httpd_req_t *req, httpd_err_code_t err)
{
    httpd_resp_set_status(req, "302 Temporary Redirect");
    httpd_resp_set_hdr(req, "Location", "/");
    // iOS requires content in the response to detect a captive portal, simply redirecting is not sufficient.
    httpd_resp_send(req, "Redirect to the captive portal", HTTPD_RESP_USE_STRLEN);

    ESP_LOGI(HTTP_SERVER_TAG, "Redirecting to root");
    return ESP_OK;
}

httpd_handle_t setup_http_server(void) {
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    httpd_handle_t server = NULL;

    if (httpd_start(&server, &config) == ESP_OK) {
        httpd_register_uri_handler(server, &root);
        httpd_register_err_handler(server, HTTPD_404_NOT_FOUND, http_404_error_handler);
    }

    return server;
}