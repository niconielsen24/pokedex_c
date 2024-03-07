#ifndef _POKE_H_
#define _POKE_H_
#include <stdlib.h>

typedef struct {
    char* name;
    char* abs;
    char* forms;
    char* held_items;
    char* game_indices;
    char* moves;
    char* stats;
    char* types;
} Poke_data;

int get_poke_data(Poke_data* data,char* text);

int clean_poke_data(Poke_data* data);

void name(char* name);

// size_t get_substr_index(char* text,char* substr);

char* get_poke_resource(char* text,char* resource);

int contains_specials_chars(char* name);

char* make_poke_url(char* pokemon);

#endif