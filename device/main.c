#include <stdio.h>
#include "pigpio.h"
#include <unistd.h>

const int PMS_5003_BAUD = 9600; // bits per second :p
const int PMS_5003_READ_BYTES = 32; 

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

void readPMSData(Data * data) {
	PmsData pmsData = { 1, 2, 3 };
	int serialHandle = serOpen("/dev/ttyS0", PMS_5003_BAUD, 0);

	for (int byteNr = 0; byteNr < PMS_5003_READ_BYTES; byteNr++) {
		int byte = serReadByte(serialHandle);
		if (byte < 0) {
			printf("something went wrong with reading byte %d from PMS, got code %d\n", byteNr, byte);
			continue;
		}

		printf("got byte %d\n", byte);
	}
	
	data->pmsData = pmsData;
	int wasSerCloseSuccessful = serClose(serialHandle);

	if (wasSerCloseSuccessful < 0) {
		printf("closing serial connection was not successful, got code %d\n", wasSerCloseSuccessful);
	}
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

	// gpioSetMode(5, PI_OUTPUT);
	
	while(0) { 
			
	}

	for (int i = 0; i < 3; i++) {
		readPMSData(dataPtr);	
	}
	

	gpioTerminate();
	return SUCCESS;
}
