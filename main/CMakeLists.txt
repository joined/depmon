file(GLOB_RECURSE FONT_SRCS ui/fonts/*.c)

idf_component_register(
    SRCS "main.cpp" ${FONT_SRCS}
    INCLUDE_DIRS "." "ui"
    REQUIRES esp_wifi esp_http_client esp_http_server nvs_flash wifi_provisioning
    EMBED_TXTFILES "data/root.html"
)
