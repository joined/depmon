#include <esp_http_server.h>
#include <esp_log.h>

#include "http_server.hpp"
#include "static_routes.hpp"

httpd_handle_t setup_http_server() {
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.uri_match_fn = httpd_uri_match_wildcard;
    httpd_handle_t server = NULL;

    if (httpd_start(&server, &config) == ESP_OK) {
        register_static_routes(server);
    }

    return server;
}