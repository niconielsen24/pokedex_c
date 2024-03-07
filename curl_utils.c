#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "curl_utils.h"

size_t write_callback(void *ptr, size_t size, size_t nmemb, void *userdata) {
    size_t real_size = size * nmemb;
    struct ResponseData *response = (struct ResponseData *)userdata;

    response->data = realloc(response->data, response->size + real_size + 1);
    if (response->data == NULL) {
        fprintf(stderr, "Failed to allocate memory\n");
        return 0;
    }

    memcpy(&(response->data[response->size]), ptr, real_size);
    response->size += real_size;
    response->data[response->size] = '\0';

    return real_size;
}

void set_curl_handle(CURL* curl,void*f,void*data,char*url){
    curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION,f);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA,data);
    curl_easy_setopt(curl, CURLOPT_URL, url);
}
