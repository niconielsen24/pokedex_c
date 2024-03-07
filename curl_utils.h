#ifndef _CURL_UTILS_H_
#define _CURL_UTILS_H_

#include <stdlib.h>
#include <curl/curl.h>

struct ResponseData {
    char *data;
    size_t size;
};

size_t write_callback(void *ptr, size_t size, size_t nmemb, void *userdata);

void set_curl_handle(CURL* curl_handle,void*fun_pointer,void*data,char*url);

#endif