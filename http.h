#ifndef HTTP_H
#define HTTP_H

int http_get(const char *pUrl, char *result, int len);
int http_post(const char *pUrl, const char *body, char *result, int len);

#endif /* HTTP_H */
