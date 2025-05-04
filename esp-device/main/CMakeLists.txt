idf_component_register(SRCS "main.c" "c/network.c" "c/http_client.c" "c/utils.c" "c/pms5003.c" "c/time.c"
                    PRIV_REQUIRES esp_http_client
                    PRIV_REQUIRES esp_wifi
                    PRIV_REQUIRES esp_driver_uart
                    PRIV_REQUIRES esp_netif
                    PRIV_REQUIRES nvs_flash
                    INCLUDE_DIRS "include")
