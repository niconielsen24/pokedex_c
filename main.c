#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/easy.h>

#include "curl_utils.h"
#include "poke.h"

int main(int argc, char** argv){
    
    if (argc != 2) {
        fprintf(stderr, "Usage %s <Address>\n",argv[0]);
        return EXIT_FAILURE;
    }
    if (contains_specials_chars(argv[1])) {
        fprintf(stderr, "Pokemon Name cannot contain special characters\n");
        return EXIT_FAILURE;
    }

    CURL* curl_h = curl_easy_init();
    if (curl_h == NULL) {
        fprintf(stderr, "Error while initializing curl handle\n");
        return EXIT_FAILURE;
    }

    char* pokemon_url = make_poke_url(argv[1]);
    struct ResponseData response_data = {0};
    set_curl_handle(curl_h,write_callback,&response_data,pokemon_url);

    CURLcode res = curl_easy_perform(curl_h);
    if (res != CURLE_OK) {
        fprintf(stderr, "Error while curl perform : %s\n",curl_easy_strerror(res));
        return EXIT_FAILURE;
    }

    char* abs = get_abilities(response_data.data);
    printf("%s\n",abs);

    // printf("%s",response_data.data);

    free(abs);
    free(pokemon_url);
    free(response_data.data);
    curl_easy_cleanup(curl_h);

    return EXIT_SUCCESS;
}