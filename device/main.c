#include <stdio.h>
#include "pigpio.h"
#include <unistd.h>

enum EXIT_CODE {
	SUCCESS = 0,
	FAILURE = 1
};

typedef struct PmsData {
	int _pm1;
	int _pm25;
 	int _pm10;
} PmsData;

typedef struct Data {
	struct PmsData pmsData;
} Data;

void setPmsDataReader(Data * data) {
	PmsData pmsData = { 1, 2, 3 };
	int handle = i2cOpen();
	
	data->pmsData = pmsData;

	i2cClose(); // TODO: some flag if this should stop? maybe on error or something
}

void clearThings() {
	int outputPins[1] = { 5 };
	for (int i = 0; i < 1; i++) {
		gpioSetPullUpDown(outputPins[i], PI_PUD_DOWN);
	}
}

int main() {
	printf("Starting some shit\n");
	int code = gpioInitialise();
	if (code < 0) {
		printf("Gpio initialization failed");
		return FAILURE;
	}

	Data data = {{}};
	Data * dataPtr =& data;

	gpioSetMode(5, PI_OUTPUT);
	
	clearThings();
	
	while(1) { 
		gpioWrite(5, 1);	
		sleep(1);
		gpioWrite(5, 0);
		sleep(1);	
	}
	

	gpioTerminate();
	return SUCCESS;
}
