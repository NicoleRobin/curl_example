/*
 * gcc -g -o curl_test ./main.c -I /home/zhangjianwei/package/curl/include -L /home/zhangjianwei/package/curl/lib -lcurl
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include "curl/curl.h"

void write_cb(char *in, uint size, uint nmemb, const char *path)
{
	FILE *pFile = fopen(path, "w");
	fwrite(in, size, nmemb, pFile);
	fclose(pFile);
}

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		printf("Usage:curl_test url\n");
		return 0;
	}

	const char *pUrl = argv[1];
	if (0 != curl_global_init(CURL_GLOBAL_ALL))
	{
		printf("curl_global_init failed\n");
		return -1;
	}
	char *pLastSlash = strrchr(pUrl, '/');

	FILE *pFile = fopen(pLastSlash + 1, "w");
	if (pFile == NULL)
	{
		printf("fopen file[%s] failed, error:%s\n", pUrl, strerror(errno));
		return -1;
	}

	CURL *pCurl = curl_easy_init();
	if (pCurl) 
	{
		CURLcode ret;
		curl_easy_setopt(pCurl, CURLOPT_URL, pUrl);
		curl_easy_setopt(pCurl, CURLOPT_VERBOSE, 1L);
		// curl_easy_setopt(pCurl, CURLOPT_WRITEFUNCTION, write_cb);
		curl_easy_setopt(pCurl, CURLOPT_WRITEDATA, pFile);

		ret = curl_easy_perform(pCurl);
		if (ret != CURLE_OK)
		{
			printf("%s\n", curl_easy_strerror(ret));
		}

		curl_easy_cleanup(pCurl);
	}

	fclose(pFile);
	curl_global_cleanup();
}
