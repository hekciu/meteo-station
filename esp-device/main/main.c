#include <stdio.h>
#include <esp_wifi_types_generic.h>
#include "nvs_flash.h"
#include "esp_log.h"

#include "freertos/FreeRTOS.h"
#include <freertos/queue.h>

#include "utils.h"
#include "dto.h"
#include "network.h"
#include "http_client.h"
#include "pms5003.h"


const char * TAG = "main";


static void error_check(esp_err_t err) {
    if (err == ESP_OK) return;

    printf("an error has occured, details: %s\n", esp_err_to_name(err));
    exit(1);
}



void app_main(void) {
    QueueHandle_t uart_queue_handle = xQueueCreate(10, sizeof(pms5003_sensor_data));

    error_check(initialize_pms5003_uart(&uart_queue_handle));

    while (1) {
        uint32_t data_size = 0;

        uint8_t data[sizeof(pms5003_sensor_data)] = {0};

        error_check(read_data_pms5003_uart(&data_size, data));

        ESP_LOGI(TAG, "got %ld bytes at uart\n", data_size);

        vTaskDelay(100);
    }

    return;

    esp_err_t err = nvs_flash_init();

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
