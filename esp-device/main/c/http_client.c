#include <stdio.h>

#include "esp_http_client.h"
#include "esp_log.h"


static const char * TAG = "http_client";


static esp_err_t _http_event_handler(esp_http_client_event_t * evt) {
    static char * output_buffer;
    static int output_len;

    switch(evt->event_id) {
        case HTTP_EVENT_ERROR:
            ESP_LOGD(TAG, "HTTP_EVENT_ERROR");
            break;
        case HTTP_EVENT_ON_CONNECTED:
            ESP_LOGD(TAG, "HTTP_EVENT_ON_CONNECTED");
            break;
        case HTTP_EVENT_HEADER_SENT:
            ESP_LOGD(TAG, "HTTP_EVENT_HEADER_SENT");
            break;
        case HTTP_EVENT_ON_HEADER:
            ESP_LOGD(TAG, "HTTP_EVENT_ON_HEADER, key=%s, value=%s", evt->header_key, evt->header_value);
            break;
        case HTTP_EVENT_ON_DATA:
            ESP_LOGD(TAG, "HTTP_EVENT_ON_DATA, not supported");
            break;
        case HTTP_EVENT_ON_FINISH:
            ESP_LOGD(TAG, "HTTP_EVENT_ON_FINISH, skipping");
            break;
        case HTTP_EVENT_DISCONNECTED:
            ESP_LOGD(TAG, "HTTP_EVENT_DISCONNECTED, skipping");
            break;
        case HTTP_EVENT_REDIRECT:
            ESP_LOGD(TAG, "HTTP_EVENT_REDIRECT");
            esp_http_client_set_header(evt->client, "From", "user@example.com");
            esp_http_client_set_header(evt->client, "Accept", "text/html");
            esp_http_client_set_redirection(evt->client);
            break;
    }

    return ESP_OK;
};


esp_err_t post_data(
    const char * host,
    const char * username,
    const char * password,
    const char * data
) {
    esp_err_t err;

    ESP_LOGI(TAG, "sending request with body: \n%s\n", data);

    // char local_response_buffer[HTTP_MAX_RESPONSE_BUFFER + 1] = {0};

    esp_http_client_config_t config = {
        .host = host,
        .path = "/pms5003",
        .port = 2317,
        .method = HTTP_METHOD_POST,
        .event_handler = _http_event_handler,
        .disable_auto_redirect = true,
        .auth_type = HTTP_AUTH_TYPE_BASIC,
        // .user_data = local_response_buffer,
        .username = username,
        .password = password,
    };

    esp_http_client_handle_t client = esp_http_client_init(&config);

    err = esp_http_client_set_post_field(client, data, strlen(data));

    if (err != ESP_OK) {
        return err;
    }

    err = esp_http_client_set_header(client, "Content-Type", "application/json");

    if (err != ESP_OK) {
        return err;
    }

    err = esp_http_client_perform(client);

    if (err != ESP_OK) {
        return err;
    }

    ESP_LOGI(TAG, "got response status code: %d\n", esp_http_client_get_status_code(client));;

    return ESP_OK;
};
