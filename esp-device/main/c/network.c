#include <stdlib.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"

#include "esp_wifi.h"
#include "esp_log.h"


static const char * TAG = "network";

static EventGroupHandle_t s_wifi_event_group;
static uint8_t s_retry_num = 0;

#define MAXIMUM_RETRY_COUNT 10

#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT BIT1


static void event_handler(void* arg, esp_event_base_t event_base,
    int32_t event_id, void* event_data) {
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
        esp_wifi_connect();
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
        if (s_retry_num < MAXIMUM_RETRY_COUNT) {
            esp_wifi_connect();
            s_retry_num++;
            ESP_LOGI(TAG, "Failed connecting to access point, retrying...");
        } else {
            xEventGroupSetBits(s_wifi_event_group, WIFI_FAIL_BIT);
            ESP_LOGI(TAG,"connecting to the access point failed");
        }
    } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
        ESP_LOGI(TAG, "got ip:" IPSTR, IP2STR(&event->ip_info.ip));
        s_retry_num = 0;
        xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
    }
}

esp_err_t initialize_network(const char * ssid, const char * password) {
    s_wifi_event_group = xEventGroupCreate();

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

    wifi_init_config_t wifi_init_config = WIFI_INIT_CONFIG_DEFAULT();

    err = esp_wifi_init(&wifi_init_config);

    if (err != ESP_OK) {
        return err;
    }

    esp_event_handler_instance_t instance_any_id;
    esp_event_handler_instance_t instance_got_ip;

    // https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/system/esp_event.html
    err = esp_event_handler_instance_register(WIFI_EVENT,
        ESP_EVENT_ANY_ID,
        &event_handler,
        NULL,
        &instance_any_id);

    if (err != ESP_OK) {
        return err;
    }

    err = esp_event_handler_instance_register(IP_EVENT,
        IP_EVENT_STA_GOT_IP,
        &event_handler,
        NULL,
        &instance_got_ip);

    if (err != ESP_OK) {
        return err;
    }

    err = esp_wifi_set_mode(WIFI_MODE_STA);

    if (err != ESP_OK) {
        return err;
    }

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

    wifi_sta_config_t wifi_station_config = {
        .threshold.authmode = WIFI_AUTH_WPA2_PSK,
        .sae_pwe_h2e = WPA3_SAE_PWE_BOTH,
        .sae_h2e_identifier = ""
    };

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

    if (err != ESP_OK) {
        return err;
    }

    // https://www.freertos.org/Documentation/02-Kernel/02-Kernel-features/06-Event-groups
    EventBits_t bits = xEventGroupWaitBits(s_wifi_event_group,
            WIFI_CONNECTED_BIT | WIFI_FAIL_BIT,
            pdFALSE,
            pdFALSE,
            portMAX_DELAY);

    if (bits & WIFI_CONNECTED_BIT) {
        ESP_LOGI(TAG, "connected to ap SSID:%s password:%s",
                 ssid, password);
    } else if (bits & WIFI_FAIL_BIT) {
        ESP_LOGI(TAG, "Failed to connect to SSID:%s, password:%s",
                 ssid, password);
        return ESP_FAIL;
    } else {
        ESP_LOGE(TAG, "UNEXPECTED EVENT");
        return ESP_FAIL;
    }

    return ESP_OK;
};
