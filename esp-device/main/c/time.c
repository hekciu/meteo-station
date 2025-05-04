#include <sys/time.h>
#include "esp_netif_sntp.h"
#include "esp_err.h"


#define SMTP_SYNC_TIMEOUT_MS 10000


esp_err_t time_synchronize_sntp() {
    esp_err_t err;

    esp_sntp_config_t config = ESP_NETIF_SNTP_DEFAULT_CONFIG("pool.ntp.org");
    err = esp_netif_sntp_init(&config);

    if (err != ESP_OK) {
        return err;
    }

    err = esp_netif_sntp_sync_wait(pdMS_TO_TICKS(SMTP_SYNC_TIMEOUT_MS));

    return err;
};


esp_err_t get_current_timestamp(uint32_t * out) {
    struct timeval tv_now;
    gettimeofday(&tv_now, NULL);

    *out = (uint32_t)tv_now.tv_sec;

    return ESP_OK;
};
