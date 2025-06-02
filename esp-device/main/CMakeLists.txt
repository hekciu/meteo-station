idf_component_register(SRCS "main.c" "c/network.c" "c/http_client.c" "c/utils.c" "c/pms5003.c" "c/time.c"
                    PRIV_REQUIRES esp_http_client
                    PRIV_REQUIRES esp_wifi
                    PRIV_REQUIRES esp_driver_uart
                    PRIV_REQUIRES esp_netif
                    PRIV_REQUIRES nvs_flash
                    INCLUDE_DIRS "include")


add_compile_definitions(DEVICE_NAME=\"$ENV{DEVICE_NAME}\")
add_compile_definitions(API_USERNAME=\"$ENV{API_USERNAME}\")
add_compile_definitions(API_PASSWORD=\"$ENV{API_PASSWORD}\")
add_compile_definitions(API_HOST=\"$ENV{API_HOST}\")
add_compile_definitions(API_PORT=$ENV{API_PORT})
add_compile_definitions(WIFI_USERNAME=\"$ENV{WIFI_USERNAME}\")
add_compile_definitions(WIFI_PASSWORD=\"$ENV{WIFI_PASSWORD}\")
