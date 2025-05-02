#include "driver/uart.h"
#include "esp_log.h"

#include "freertos/FreeRTOS.h"
#include <freertos/queue.h>


#define PMS5003_UART_BUFFER_SIZE 2048
#define PMS5003_UART_NUM UART_NUM_2
#define PMS5003_UART_PIN_RX 1
#define PMS5003_UART_PIN_TX 3
#define PMS5003_BAUD_RATE 9600
#define PMS5003_RTS_THRESHOLD 122


esp_err_t initialize_pms5003_uart(QueueHandle_t * queue_handle) {
    esp_err_t err;

    err = uart_driver_install(PMS5003_UART_NUM, PMS5003_UART_BUFFER_SIZE, PMS5003_UART_BUFFER_SIZE, 10, queue_handle, 0);

    if (err != ESP_OK) {
        return err;
    }

    uart_config_t uart_config = {
        .baud_rate = PMS5003_BAUD_RATE,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .rx_flow_ctrl_thresh = PMS5003_RTS_THRESHOLD
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

    *size = uart_read_bytes(PMS5003_UART_NUM, data, *size, 100);

    err = uart_flush(PMS5003_UART_NUM);

    return err;
}



