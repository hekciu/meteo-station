#ifndef DEVICE_UTILS
#define DEVICE_UTILS

size_t takeSmallerSize(size_t a, size_t b) {
	if (a < b) {
		return a;
	}

	return b;
}

#endif
