file(GLOB_RECURSE FONT_SRCS ui/fonts/*.c)

idf_component_register(
    SRCS "utils.cpp" "bvg_api_client.cpp" "lcd.cpp" "main.cpp" "http_server.cpp" "ui/ui.cpp" "time.cpp" ${FONT_SRCS}
    INCLUDE_DIRS "." "ui"
    PRIV_REQUIRES esp_app_format esp_http_client esp_http_server esp_timer esp_wifi json nvs_flash spiffs vfs wifi_provisioning lwip
)

set(WEB_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../frontend")
if(EXISTS ${WEB_SRC_DIR}/dist)
    spiffs_create_partition_image(www ${WEB_SRC_DIR}/dist FLASH_IN_PROJECT)
else()
    message(FATAL_ERROR "${WEB_SRC_DIR}/dist doesn't exit. Please run 'yarn build' in ${WEB_SRC_DIR}")
endif()
