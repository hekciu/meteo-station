#include <stdint.h>

#define MAX_PG_QUERY_SIZE 1024

extern int get_PMS5003_measurements(uint64_t timestampFrom, uint64_t timestampTo, char ** output);

extern int insert_PMS5003_measurement(uint64_t device_timestamp, char * device_name, uint16_t pm10_standard, uint16_t pm25_standard, uint16_t pm100_standard);

extern const char * DEVICE_TIMESTAMP_FN;
extern const char * DEVICE_NAME_FN;
extern const char * PM10_STANDARD_FN;
extern const char * PM25_STANDARD_FN; 
extern const char * PM100_STANDARD_FN;
