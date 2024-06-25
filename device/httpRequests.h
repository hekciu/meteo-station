/*
	This is designed to be simple one-header set of functions for making http requests
*/

#include <string.h>
#include <curl/curl.h>
#include <stdbool.h>


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

	res = curl_easy_perform();
	
	if (res == 0) {
		return true;
	}

	return false;
}

