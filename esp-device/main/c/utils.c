#include "dto.h"
#include <stdio.h>


uint16_t transform_bytes(uint8_t high, uint8_t low) {
    uint16_t output = low;
    output += (high << 8);

    return output;
};


void measurement_to_json(const pms5003_measurement * measurement, char * output, int n) {
    int _ = snprintf(output,
        n,
        "{ \
        \"DeviceTimestamp\": %lu,\
        \"DeviceName\": \"%s\",\
        \"Pm10Standard\": %u,\
        \"Pm25Standard\": %u,\
        \"Pm100Standard\": %u} ",
        measurement->device_timestamp,
        measurement->device_name,
        measurement->pm10_standard,
        measurement->pm25_standard,
        measurement->pm100_standard);
};
