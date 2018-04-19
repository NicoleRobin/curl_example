/*
 * gcc -o test_http ./test_http.c ./http.c -I /home/zhangjianwei/package/curl/include -L /home/zhangjianwei/package/curl/lib -lcurl
 */
#include <stdio.h>

#include "http.h"

int main(int argc, char **argv)
{
	if (argc != 2) {
		printf("Usage:test_http url\n");
		return 0;
	}

	const char *pUrl = argv[1];
	char result[10240] = "";
	int iRet = http_get(pUrl, result, 10239);
	if (iRet != 0) {
		printf("http_get failed()\n");
		return -1;
	}

	printf("Result:%s\n", result);
	return 0;
}
