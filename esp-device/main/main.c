#include <stdio.h>
#include <esp_wifi_types_generic.h>
#include "nvs_flash.h"

#include "utils.h"
#include "types.h"
#include "network.h"
#include "http_client.h"


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

    // ladne106 działa, aladne106 nie
    error_check(initialize_network("", ""));

    pms5003_measurement test = {
        .device_timestamp = 696969,
        .device_name = "dupadupa",
        .pm10_standard = 123,
        .pm25_standard = 221,
        .pm100_standard = 231
    };

    char * data = malloc(301);

    measurement_to_json(&test, data, 300);

    printf("sending data :\n%s\n", data);

    error_check(post_data("192.168.1.148", "dev", "dev", data));

    free(data);
}
