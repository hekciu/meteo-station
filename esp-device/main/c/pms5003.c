#include <stdlib.h>

#include "driver/uart.h"
#include "esp_log.h"

#include "pms5003.h"

#include "freertos/FreeRTOS.h"
#include <freertos/queue.h>


#define PMS5003_UART_BUFFER_SIZE 2048
#define PMS5003_UART_NUM UART_NUM_2
#define PMS5003_UART_PIN_RX 0
#define PMS5003_UART_PIN_TX 1
#define PMS5003_BAUD_RATE 9600


esp_err_t initialize_pms5003_uart() {
    esp_err_t err;

    err = uart_driver_install(PMS5003_UART_NUM, PMS5003_UART_BUFFER_SIZE, PMS5003_UART_BUFFER_SIZE, 0, NULL, 0);

    if (err != ESP_OK) {
        return err;
    }

    uart_config_t uart_config = {
        .baud_rate = PMS5003_BAUD_RATE,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_DEFAULT,
    };

    err = uart_param_config(PMS5003_UART_NUM, &uart_config);

    if (err != ESP_OK) {
        return err;
    }

    err = uart_set_pin(PMS5003_UART_NUM, PMS5003_UART_PIN_TX, PMS5003_UART_PIN_RX, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);

    return err;
}



esp_err_t read_data_pms5003_uart(uint32_t * size, uint8_t * data) {
    esp_err_t err;

    err = uart_get_buffered_data_len(PMS5003_UART_NUM, (size_t *)size);

    if (err != ESP_OK) {
        return err;
    }

    if (*size >= sizeof(pms5003_sensor_data)) {
        *size = uart_read_bytes(PMS5003_UART_NUM, data, sizeof(pms5003_sensor_data), 100);
    }

    err = uart_flush(PMS5003_UART_NUM);

    return err;
}



