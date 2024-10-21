#include <stdint.h>

#define MAX_PG_QUERY_SIZE 1024

int _createConnInfoFromEnvs();

size_t get_measurements(uint64_t timestampFrom, uint64_t timestampTo);
