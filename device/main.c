#include <stdio.h>
#include "pigpio.h"
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "httpRequests.h"
#include "config.h"
#include "../common/get_current_timestamp.h"

#define PMS_5003_BAUD 9600
#define PMS_5003_READ_BYTES 32
#define PMS_START_BYTE 0x42
#define MAX_OUTPUT_DATA_LENGTH 1024


enum EXIT_CODE {
	SUCCESS = 0,
	FAILURE = 1
};

struct PmsData {
  uint16_t framelen;
  uint16_t pm10_standard, pm25_standard, pm100_standard;
  uint16_t pm10_env, pm25_env, pm100_env;
  uint16_t particles_03um, particles_05um, particles_10um, particles_25um, particles_50um, particles_100um;
  uint16_t unused;
  uint16_t checksum;
};

struct Data {
	struct PmsData pmsData;
};

size_t createPMS5003Data(struct Data * data, char * output, char * deviceName) {
	uint32_t timestamp = get_current_timestamp();

	size_t size = snprintf(output, MAX_OUTPUT_DATA_LENGTH, "device_name=%s&device_timestamp=%d&pm10_standard=%hu&pm25_standard=%hu&pm100_standard=%hu", deviceName, timestamp, data->pmsData.pm10_standard, data->pmsData.pm25_standard, data->pmsData.pm100_standard);	

	return size;
}

void test() {
	int serialHandle = serOpen("/dev/serial0", 9600, 0);

	while (1) {
		int bytesAvailable = serDataAvailable(serialHandle); 

		printf("bytes available: %d\n", bytesAvailable);

		sleep(2);
	}

}

bool readPMSData(struct Data * data) {
	struct PmsData pmsData = { 1, 2, 3 };
	int serialHandle = serOpen("/dev/serial0", PMS_5003_BAUD, 0);

	if (serialHandle < 0) {
		printf("opening serial connection failed with %d code\n", serialHandle);
		return false;
	}

	int antiInf = 0;

	while (antiInf < 20) {
		sleep(1);
		antiInf++;
		int bytesAvailable = serDataAvailable(serialHandle);
	
		if (bytesAvailable == 0) {
			continue;
		}
	
		for (int b = 0; b < bytesAvailable; b++) {
			uint8_t firstByte = serReadByte(serialHandle);

			if (firstByte != PMS_START_BYTE) {
				continue;
			};

			int bytesLeft = bytesAvailable - b;
			if (bytesLeft < PMS_5003_READ_BYTES) {
				printf("too few bytes: %d < %d\n", bytesLeft, PMS_5003_READ_BYTES);
				return false;
			}

			uint8_t bytes[32];
			bytes[0] = firstByte;
			uint16_t checkSum = firstByte;
		
			for (int byteNr = 1; byteNr < PMS_5003_READ_BYTES; byteNr++) {
				int byte = serReadByte(serialHandle);
				if (byte < 0) {
					continue;
				}
				
				bytes[byteNr] = byte;

				if (byteNr < 30) {
					checkSum += byte;
				}
			}
			
			uint16_t buffer_u16[15];

			for (uint8_t i = 0; i < 15; i++) {
				buffer_u16[i] = bytes[2 + i*2 + 1];
				buffer_u16[i] += (bytes[2 + i*2] << 8);
			}		

			memcpy((void *) &data->pmsData, (void *) buffer_u16, 30); 
			
			if (data->pmsData.checksum != checkSum) {
				return false;
			}
			
			return true;
		}
	}

	

	data->pmsData = pmsData;
	int wasSerCloseSuccessful = serClose(serialHandle);

	if (wasSerCloseSuccessful < 0) {
		printf("closing serial connection was not successful, got code %d\n", wasSerCloseSuccessful);
	}
}

int main(int argc, char * argv[]) {
	printf("Starting some shit ngl\n");

	char * serverUrl = getenv("SERVER_URL");
	char * serverAuthEncoded = getenv("SERVER_AUTH_ENCODED");
    // TODO: add requirement for max 20 characters in device name as it's the max one in db
	char * deviceName = getenv("DEVICE_NAME");

	int code = gpioInitialise();
	if (code < 0) {
		printf("Gpio initialization failed code: %d \n", code);
		return FAILURE;
	}

	struct Data data = {{}};

	 for (;;) {
		if(readPMSData(&data)) {
			printf("data %d %d %d\n", data.pmsData.pm10_standard, data.pmsData.pm25_standard, data.pmsData.pm100_standard);

			printf("creating data structure\n");
			char * output = malloc(MAX_OUTPUT_DATA_LENGTH);
			size_t outputSize = createPMS5003Data(&data, output, deviceName);
			printf("createdData: %s\n", output);
			printf("sending data to server\n");
			if(postData(serverUrl, output, serverAuthEncoded, "application/x-www-form-urlencoded")) {
				printf("success!!\n");
			} else {
				printf("failure\n");
			}

			free(output);
		}	
		sleep(MEASUREMENTS_INTERVAL_SEC);
	}
	

	gpioTerminate();
	return SUCCESS;
}
