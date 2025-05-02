#include <stdint.h>

#include "freertos/FreeRTOS.h"
#include <freertos/queue.h>


#define PMS5003_FIRST_CHAR 0x42
#define PMS5003_SECOND_CHAR 0x4d


typedef struct {
    char first_char;
    char second_char;
    uint8_t frame_length_high;
    uint8_t frame_length_low;
    uint8_t data_1_high;
    uint8_t data_1_low;
    uint8_t data_2_high;
    uint8_t data_2_low;
    uint8_t data_3_high;
    uint8_t data_3_low;
    uint8_t data_4_high;
    uint8_t data_4_low;
    uint8_t data_5_high;
    uint8_t data_5_low;
    uint8_t data_6_high;
    uint8_t data_6_low;
    uint8_t data_7_high;
    uint8_t data_7_low;
    uint8_t data_8_high;
    uint8_t data_8_low;
    uint8_t data_9_high;
    uint8_t data_9_low;
    uint8_t data_10_high;
    uint8_t data_10_low;
    uint8_t data_11_high;
    uint8_t data_11_low;
    uint8_t data_12_high;
    uint8_t data_12_low;
    uint8_t data_13_high;
    uint8_t data_13_low;
    uint8_t data_and_check_high;
    uint8_t data_and_check_low;
} pms5003_sensor_data;


esp_err_t initialize_pms5003_uart(QueueHandle_t * queue_handle);
esp_err_t read_data_pms5003_uart(uint32_t * size, uint8_t * data);
