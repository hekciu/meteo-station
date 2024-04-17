#include <stdio.h>
#include "pigpio.h"

int main() {
	printf("Starting some shit\n");
	int code = gpioInitialise();
	if (code < 0) {
		printf("Gpio initialization failed");
		return 1;
	}


	gpioTerminate();
	return 0;
}
