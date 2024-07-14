#include "httpRequests.h"

int main() { 
	char * outputStringPtr = NULL;
	size_t size = getStringData("localhost:2137/healthcheck", &outputStringPtr); 
	printf("%s\n", outputStringPtr);
	postStringData("localhost:2137/data", "{ \"data\": {} }");
	return 0;
}
