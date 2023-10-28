# Embed the frontend files into the binary
include(${COMPONENT_DIR}/data/source_embedded_files.cmake)

idf_component_register(SRCS http_server.cpp
                       INCLUDE_DIRS include
                       REQUIRES esp_http_server
                       EMBED_TXTFILES ${FRONTEND_FILES_TO_EMBED})