/*
	This is designed to be simple one-headered set of functions for making http requests
*/

#include <string.h>
#include <curl/curl.h>
#include <stdbool.h>
#include <stdlib.h>

struct MemoryStruct {
	char * memory;
	size_t size;
};

size_t WriteMemoryCallback(char * input, size_t size, size_t nmemb, void * userdata) {
	size_t realsize = size * nmemb;

	struct MemoryStruct * mem = (struct MemoryStruct *)userdata;
	
	char *ptr = realloc(mem->memory, mem->size + realsize + 1);
	if(!ptr) {
		printf("not enough memory, realloc returned NULL\n");
		return 0;
	}

	mem->memory = ptr;
	memcpy(&(mem->memory[mem->size]), input, realsize);
	mem->size += realsize;
	mem->memory[mem->size] = 0;

	return realsize;
}

bool postStringData(char* url, char* data) {
	CURL * curl_handle;
	CURLcode res;

	curl_handle = curl_easy_init();

	if(!curl_handle) {
		return false;
	}

	curl_easy_setopt(curl_handle, CURLOPT_URL, url);
	curl_easy_setopt(curl_handle, CURLOPT_FOLLOWLOCATION, 1);
	// curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
	// curl_easy_setopt(curl_handle, CURLOPT_READFUNCTION, ReadMemoryCallback);
	curl_easy_setopt(curl_handle, CURLOPT_POST, 1);
	curl_easy_setopt(curl_handle, CURLOPT_POSTFIELDS, data);
	curl_easy_setopt(curl_handle, CURLOPT_POSTFIELDSIZE, strlen(data));
	curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "meteo-station-edge/1.0.0");

	res = curl_easy_perform(curl_handle);
	curl_easy_cleanup(curl_handle);
	
	if (res == CURLE_OK) {
		return true;
	}

	return false;
}

bool getStringData(char * url, char * outputString, size_t allocatedSize) {
	CURL * curl_handle;
	CURLcode res;

	curl_handle = curl_easy_init();

	if(!curl_handle) {
		return false;
	}

	struct MemoryStruct outputData;
	outputData.memory = malloc(1);
	outputData.size = 0;

	curl_easy_setopt(curl_handle, CURLOPT_URL, url);
	curl_easy_setopt(curl_handle, CURLOPT_FOLLOWLOCATION, 1);
	curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
	curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&outputData);
	// curl_easy_setopt(curl_handle, CURLOPT_READFUNCTION, ReadMemoryCallback);
	curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "meteo-station-edge/1.0.0");

	res = curl_easy_perform(curl_handle);
	curl_easy_cleanup(curl_handle);

	
	if (res == CURLE_OK) {
		printf("%lu bytes retrieved\n", (unsigned long)outputData.size);
		if (allocatedSize < outputData.size) {
			printf("output buffer too small %lu > %lu", (unsigned long)allocatedSize, (unsigned long) outputData.size);
			free(outputData.memory);
			return false;
		}
		memcpy(outputString, outputData.memory, outputData.size);
		free(outputData.memory);
		return true;
	}

	free(outputData.memory);
	return false;
}

