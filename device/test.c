#include "httpRequests.h"

int main() { 
	const int size = 2;
	char outputString[size];
	getStringData("localhost:2137/healthcheck", outputString, size); 
	printf(outputString);
	return 0;
}
