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
#include "time.h"


const char * TAG = "main";

#define PMS5003_WAIT_MS 1000
#define PMS5003_TASK_STACK_SIZE 4096
#define PMS5003_MAX_JSON_SIZE 1000


static inline void error_check(esp_err_t err) {
    if (err == ESP_OK) return;

    ESP_LOGE(TAG, "an error has occured, details: %s\n", esp_err_to_name(err));
    exit(1);
}


static void vTaskPms5003(void * _) {
    while (1) {
        uint32_t data_size = 0;

        pms5003_sensor_data sensor_data;

        error_check(read_data_pms5003_uart(&data_size, (uint8_t *)&sensor_data));

        ESP_LOGI(TAG, "got %ld bytes at uart\n", data_size);

        if (data_size >= sizeof(pms5003_sensor_data)) {
            if (pms5003_validate_data(sensor_data) == 0) {
                uint32_t timestamp = 0;

                error_check(get_current_timestamp(&timestamp));

                pms5003_measurement measurement = {
                    .device_timestamp = timestamp,
                    .device_name = "AAAAA",
                    .pm10_standard = transform_bytes(sensor_data.data_1_high, sensor_data.data_1_low),
                    .pm25_standard = transform_bytes(sensor_data.data_2_high, sensor_data.data_2_low),
                    .pm100_standard = transform_bytes(sensor_data.data_3_high, sensor_data.data_3_low),
                };

                ESP_LOGI(TAG, "got data:\n");
                ESP_LOGI(TAG, "pm10: %d, pm25: %d, pm100: %d\n",
                    measurement.pm10_standard,
                    measurement.pm25_standard,
                    measurement.pm100_standard);


                char data[PMS5003_MAX_JSON_SIZE] = {0};

                measurement_to_json(&measurement, data, PMS5003_MAX_JSON_SIZE);

                ESP_LOGI(TAG, "sending data :\n%s\n", data);

                error_check(post_data("192.168.1.148", "dev", "dev", data));
            } else {
                ESP_LOGI(TAG, "got some malformed data");
            }
        }

        vTaskDelay(PMS5003_WAIT_MS / portTICK_PERIOD_MS);
    }
};


void app_main(void) {
    esp_err_t err = nvs_flash_init();

    if (err != ESP_OK) {
        nvs_flash_erase();
        error_check(nvs_flash_init());
    }

    // ladne106 działa, aladne106 nie
    error_check(initialize_network("", ""));
    error_check(time_synchronize_sntp());
    error_check(initialize_pms5003_uart());

    xTaskCreate(vTaskPms5003, "Pms5003", PMS5003_TASK_STACK_SIZE, NULL, configMAX_PRIORITIES - 1, NULL);

}
