#include <stdint.h>

#define MAX_PG_QUERY_SIZE 1024

int get_PMS5003_measurements(uint64_t timestampFrom, uint64_t timestampTo, char ** output);

int insert_PMS5003_measurement(char * tableName, char * data);

