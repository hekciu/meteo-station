#include <stdint.h>

extern int parseInsertPMS5003Body(char * body, uint64_t * device_timestamp, char ** device_name, uint16_t * pm10_standard, uint16_t * pm25_standard, uint16_t * pm100_standard);
