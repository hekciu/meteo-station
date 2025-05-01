idf_component_register(SRCS "main.c" "c/network.c" "c/http_client.c" "c/utils.c"
                    PRIV_REQUIRES esp_http_client
                    PRIV_REQUIRES esp_wifi
                    PRIV_REQUIRES nvs_flash
                    INCLUDE_DIRS "include")
