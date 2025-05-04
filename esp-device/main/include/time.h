#include "esp_err.h"


esp_err_t time_synchronize_sntp();
esp_err_t get_current_timestamp(uint32_t * out);
