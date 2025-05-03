#include "dto.h"


uint16_t transform_bytes(uint8_t high, uint8_t low);
void measurement_to_json(const pms5003_measurement * measurement, char * output, int n);
