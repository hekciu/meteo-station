#ifndef UTILS
#define UTILS

#include <time.h>
#include <stdint.h>

size_t takeSmallerSize(size_t a, size_t b) {
	if (a < b) {
		return a;
	}

	return b;
}

uint32_t getCurrentTimestamp() {
	return (uint32_t)time(NULL);
}

#endif
