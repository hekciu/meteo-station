#include <stdlib.h>
#include <string.h>

#include "esp_wifi.h"
#include "esp_log.h"


static const char * TAG = "network";


#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT BIT1


esp_err_t initialize_network(const char * ssid, const char * password) {
    esp_err_t err;

    err = esp_netif_init();

    if (err != ESP_OK) {
        return err;
    }

    err = esp_event_loop_create_default();

    if (err != ESP_OK) {
        return err;
    }

    esp_netif_create_default_wifi_sta();

    wifi_init_config_t wifi_config = WIFI_INIT_CONFIG_DEFAULT();

    err = esp_wifi_init(&wifi_config);

    if (err != ESP_OK) {
        return err;
    }

    esp_event_handler_instance_t instance_any_id;
    esp_event_handler_instance_t instance_got_ip;

    // https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/system/esp_event.html
    err = esp_event_hander_instance_register(WIFI

    if (err != ESP_OK) {
        return err;
    }

    err = esp_event_hander_instance_register(WIFI

    if (err != ESP_OK) {
        return err;
    }

    err = esp_wifi_set_mode(WIFI_MODE_STA);

    wifi_sta_config_t wifi_station_config = {};

    size_t ssid_len = strlen(ssid) + 1;
    size_t password_len = strlen(password) + 1;

    if (ssid_len > 32) {
        ESP_LOGE(TAG, "provided ssid is too long");
        exit(1);
    }

    if (ssid_len > 64) {
        ESP_LOGE(TAG, "provided password is too long");
        exit(1);
    }

    memcpy(&wifi_station_config.ssid, ssid, ssid_len);
    memcpy(&wifi_station_config.password, password, password_len);

    wifi_config_t wifi_config = {
        .sta = wifi_station_config
    };

    err = esp_wifi_set_config(WIFI_IF_STA, &wifi_config);

    if (err != ESP_OK) {
        return err;
    }

    err = esp_wifi_start();

    return err;
};
