#include <stdint.h>

#ifndef TYPES
#define TYPES

typedef struct {
    uint32_t device_timestamp;
    char device_name[20];
    uint16_t pm10_standard;
    uint16_t pm25_standard;
    uint16_t pm100_standard;
} pms5003_measurement;

#endif
