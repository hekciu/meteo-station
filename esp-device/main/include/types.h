#include <stdint.h>

#ifndef TYPES
#define TYPES

typedef struct {
    uint32_t deviceTimestamp;
    char deviceName[20];
    uint8_t pm10_standard;
    uint8_t pm25_standard;
    uint8_t pm100_standard;
} pms5003_measurement;

#endif
