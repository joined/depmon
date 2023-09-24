idf_component_register(
    SRCS "main.cpp"
    INCLUDE_DIRS "."
    REQUIRES esp_wifi
    REQUIRES esp_http_client
    PRIV_REQUIRES ui
)
