#include <time.h>

uint32_t get_current_timestamp() {
    return (uint32_t)time(NULL);
}
