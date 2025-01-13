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
#include "pms5003.h"

#define MAX_OUTPUT_DATA_LENGTH 1024
#define MAX_DEVICE_NAME_LENGTH 20


struct Data {
	struct Pms5003Data pmsData;
};

int main(int argc, char * argv[]) {
	printf("Starting some shit ngl\n");

	char * serverUrl = getenv("SERVER_URL");
	char * serverAuthEncoded = getenv("SERVER_AUTH_ENCODED");
	char * deviceName = getenv("DEVICE_NAME");

    if (strlen(deviceName) > MAX_DEVICE_NAME_LENGTH) {
        printf("DEVICE_NAME variable should not exceed %d characters", MAX_DEVICE_NAME_LENGTH);
        return -1;
    }

	int code = gpioInitialise();
	if (code < 0) {
		printf("Gpio initialization failed code: %d \n", code);
		return -1;
	}

	struct Data data = {{}};

	 for (;;) {
		if(readPMS5003Data(&data)) {
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
	return 0;
}
