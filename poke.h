#ifndef _POKE_H_
#define _POKE_H_
#include <stdlib.h>

typedef struct {
    char* name;
    char* abilities;
} Pokemon;

// size_t get_substr_index(char* text,char* substr);

char* get_abilities(char* text);

int contains_specials_chars(char* name);

char* make_poke_url(char* pokemon);

#endif