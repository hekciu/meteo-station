#include <stdio.h>
#include <esp_wifi_types_generic.h>
#include "esp_http_client.h"
#include "nvs_flash.h"

#include "types.h"
#include "network.h"


static void error_check(esp_err_t err) {
    if (err == ESP_OK) return;

    printf("an error has occured, details: %s\n", esp_err_to_name(err));
    exit(1);
}



void app_main(void) {
    esp_err_t err;

    err = nvs_flash_init();

    if (err != ESP_OK) {
        nvs_flash_erase();
        error_check(nvs_flash_init());
    }

    error_check(initialize_network("dupa", "dupa"));

    // esp_err_t err = post_data("hello world!");
}
