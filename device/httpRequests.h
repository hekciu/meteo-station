/*
	This is designed to be simple one-headered set of functions for making http requests
*/

#include <string.h>
#include <curl/curl.h>
#include <stdbool.h>
#include <stdlib.h>

#include "utils.h"

struct MemoryStruct {
	char * memory;
	size_t size;
	size_t itr;
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
	mem->memory[mem->size] = '\0';

	return realsize;
}

size_t ReadMemoryCallback(char * ptr, size_t size, size_t nmemb, void * userdata) {
	size_t sizeAvailable = size * nmemb;
	
	struct MemoryStruct* mem = (struct MemoryStruct*)userdata;

	size_t dataSize = takeSmallerSize(mem->size, sizeAvailable);	

	if (mem->itr == mem->size) {
		return 0;
	} else {
		mem->itr += dataSize;
	}

	memcpy(ptr, mem->memory, dataSize);	
	return dataSize;
}

bool postData(char * url, char * data, char * authHeader, char * contentType) {
	CURL * curl_handle;
	CURLcode res;

	curl_handle = curl_easy_init();

	if(!curl_handle) {
		return false;
	}

	// reading memory by chunks is disabled by now, POSTFIELDS is more than enough for around 100 characters
	/*
	struct MemoryStruct inputData;
	inputData.size = strlen(data);
	inputData.memory = malloc(inputData.size);
	memcpy(inputData.memory, data, inputData.size);
	inputData.itr = 0;
	*/

	struct curl_slist * headers = NULL;
    const char * contentTypeHeaderFormat = "Content-Type: %s";
    char * contentTypeHeader = malloc(strlen(contentTypeHeaderFormat) + strlen(contentType) -1 + 1);
    sprintf(contentTypeHeader, contentTypeHeaderFormat, contentType);
	headers = curl_slist_append(headers, contentTypeHeader);
	if (authHeader != NULL) {
		char * authString = "Authorization: ";
		size_t headerStringSize = strlen(authString) + strlen(authHeader) + 1;
		char * authHeaderString = malloc(headerStringSize);
		memcpy(authHeaderString, authString, strlen(authString));
		memcpy(authHeaderString + strlen(authString), authHeader, strlen(authHeader));
		authHeaderString[headerStringSize - 1] = '\0';
		headers = curl_slist_append(headers, authHeaderString);
		free(authHeaderString);
	}

	if (!headers) {
		/*
		free(inputData.memory);
		*/
		return false;
	}

	curl_easy_setopt(curl_handle, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_1_0);
	curl_easy_setopt(curl_handle, CURLOPT_URL, url);
	curl_easy_setopt(curl_handle, CURLOPT_FOLLOWLOCATION, 1);
	/*
	curl_easy_setopt(curl_handle, CURLOPT_READFUNCTION, ReadMemoryCallback);
	curl_easy_setopt(curl_handle, CURLOPT_READDATA, (void*)&inputData);
	*/
	curl_easy_setopt(curl_handle, CURLOPT_POSTFIELDS, data);
	curl_easy_setopt(curl_handle, CURLOPT_POST, 1);
	curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "meteo-station-edge/0.2.0");
	curl_easy_setopt(curl_handle, CURLOPT_HTTPHEADER, headers);

	res = curl_easy_perform(curl_handle);

	curl_slist_free_all(headers);
	curl_easy_cleanup(curl_handle);
	/*
	free(inputData.memory);
	*/

	if (res == CURLE_OK) {
		return true;
	} else {
		fprintf(stderr, "error, got curl code: %d\n", res);
	}

	return false;
}

size_t getStringData(char * url, char ** outputStringPtr) {
	CURL * curl_handle;
	CURLcode res;

	curl_handle = curl_easy_init();

	if(!curl_handle) {
		return -1;
	}

	struct MemoryStruct outputData;
	outputData.memory = NULL;
	outputData.size = 0;

	curl_easy_setopt(curl_handle, CURLOPT_URL, url);
	curl_easy_setopt(curl_handle, CURLOPT_FOLLOWLOCATION, 1);
	curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
	curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&outputData);
	curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "meteo-station-edge/1.0.0");

	res = curl_easy_perform(curl_handle);
	curl_easy_cleanup(curl_handle);

	
	if (res == CURLE_OK) {
		size_t dataSize = outputData.size + 1;
		*outputStringPtr = (char*) malloc(dataSize);
		memcpy(*outputStringPtr, outputData.memory, dataSize);
		free(outputData.memory);
		return outputData.size;
	}

	free(outputData.memory);
	return -1;
}

