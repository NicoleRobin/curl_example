#include "http.h"

#include "curl/curl.h"

#include <stdio.h>

struct STWriteData
{
	char *pData;
	unsigned int len;
};

size_t write_cb(char *ptr, size_t size, size_t nmemb, void *arg)
{
	printf("In write_cb, in:%p, size:%u, umemb:%u\n", ptr, size, nmemb);
	struct STWriteData *pWriteData = (struct STWriteData*)arg;
	unsigned int uMax = (size * nmemb) > pWriteData->len ? (size * nmemb) : pWriteData->len;
	snprintf(pWriteData->pData, uMax, "%s", ptr);
}

int http_get(const char *pUrl, char *result, int len)
{
	// printf("In http_get, url:%s\n", pUrl);
	if (0 != curl_global_init(CURL_GLOBAL_ALL)) {
		printf("In http_get, curl_global_init() failed\n");
		return -1;
	}

	CURL *pCurl = curl_easy_init();
	if (!pCurl) {
		printf("In http_get, curl_easy_init() failed\n");
		return -2;
	}
	else {
		CURLcode ret;
		struct STWriteData writeData;
		writeData.pData = result;
		writeData.len = len;
		ret = curl_easy_setopt(pCurl, CURLOPT_URL, pUrl);
		if (ret != CURLE_OK) {
			printf("In http_get, curl_easy_setopt() failed, error:%s\n", curl_easy_strerror(ret));
			return ret;
		}

		/*
		ret = curl_easy_setopt(pCurl, CURLOPT_HTTPGET, 1L);
		if (ret != CURLE_OK) {
			printf("In http_get, curl_easy_setopt() failed, error:%s\n", curl_easy_strerror(ret));
			return ret;
		}
		*/
		ret = curl_easy_setopt(pCurl, CURLOPT_FOLLOWLOCATION, 1L);
		if (ret != CURLE_OK) {
			printf("In http_get, curl_easy_setopt() failed, error:%s\n", curl_easy_strerror(ret));
			return ret;
		}

		ret = curl_easy_setopt(pCurl, CURLOPT_WRITEFUNCTION, write_cb);
		if (ret != CURLE_OK) {
			printf("In http_get, curl_easy_setopt() failed, error:%s\n", curl_easy_strerror(ret));
			return ret;
		}

		ret = curl_easy_setopt(pCurl, CURLOPT_WRITEDATA, &writeData); 
		if (ret != CURLE_OK) {
			printf("In http_get, curl_easy_setopt() failed, error:%s\n", curl_easy_strerror(ret));
			return ret;
		}

		ret = curl_easy_perform(pCurl);
		if (ret != CURLE_OK) {
			printf("In http_get, curl_easy_perform() failed, %s\n", curl_easy_strerror(ret));
			return ret;
		}
		curl_easy_cleanup(pCurl);
	}

	curl_global_cleanup();
	return 0;
}

int http_post(const char *pUrl, const char *body, char *result, int len)
{
	return 0;
}
