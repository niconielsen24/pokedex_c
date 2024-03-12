#include "poke.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>

static size_t get_substr_index(char *text, char *substr) {
    size_t length = strlen(text);
    size_t substr_len = strlen(substr);
    for (size_t txt_index = 0; txt_index < length; txt_index++) {
      if (text[txt_index] == substr[0]) {
        size_t substr_index = 0;
        while (text[txt_index + substr_index] == substr[substr_index] &&
               substr_index < substr_len)
          substr_index++;
        if (substr_index == substr_len)
          return txt_index;
        else
          txt_index += substr_index;
      }
    }
    return SIZE_MAX;
}

char *get_poke_resource(char *text,char* resource) {
    char *res = NULL;
    size_t index = get_substr_index(text, resource);
    if (index == SIZE_MAX)
      return NULL;
    size_t len = strlen(text);
    int i = 0;
    while (text[index + i] != ']' && (index + i) < len)
      i++;
    if (index + i < len)
      i++;
    res = malloc((i + 1) * sizeof(char));
    for (int j = 0; j < i; j++) {
      res[j] = text[index + j];
    }
    res[i] = '\0';
    for (int z = 0; z<i; z++) {
      if( res[z] == '{' || res[z] == '}' || res[z] == '(' || 
          res[z] == ')' || res[z] == '[' || res[z] == ']' ||
          res[z] == '"' || res[z] == '_'
        ) res[z] = ' ';
      else if(res[z] == ':') res[z+1] = '\n';
    }
    return res;
}

static bool is_special(char c) {
    char *specials = ";:/?&^#@!(){}[]|-_=+*$`~.,";
    for (int i = 0; i < 27; i++) {
      if (c == specials[i])
        return true;
    }
    return false;
}

int contains_specials_chars(char *name) {
    size_t length = strlen(name);
    for (size_t i = 0; i < length; i++) {
      if (is_special(name[i])) {
        return EXIT_FAILURE;
      }
    }
    return EXIT_SUCCESS;
}

char *make_poke_url(char *pokemon) {
    char *url = "https://pokeapi.co/api/v2/pokemon/";
    size_t name_length = strlen(pokemon);
    char *pokemon_url = malloc((35 + name_length + 1) * sizeof(char));
    strcpy(pokemon_url, url);
    strcat(pokemon_url, pokemon);
    return pokemon_url;
}

int get_poke_data(Poke_data* data,char* text){
    data->abs = get_poke_resource(text,"abilities");
    data->forms = get_poke_resource(text,"forms");
    data->held_items = get_poke_resource(text,"held_items");
    data->game_indices = get_poke_resource(text,"game_indices");
    data->moves = get_poke_resource(text,"moves");
    data->stats = get_poke_resource(text,"stats");
    data->types = get_poke_resource(text,"types");

    if (!data->abs || !data->forms || !data->held_items || !data->game_indices || !data->moves || !data->stats || !data->types) {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

int clean_poke_data(Poke_data* data){
    free(data->abs);
    free(data->forms);
    free(data->held_items);
    free(data->game_indices);
    free(data->moves);
    free(data->stats);
    free(data->types);

    data->abs = NULL;
    data->forms = NULL;
    data->held_items = NULL;
    data->game_indices = NULL;
    data->moves = NULL;
    data->stats = NULL;
    data->types = NULL;

    return EXIT_SUCCESS;
}

int cool_name(char ch, int letter_hight, int line){
     if (ch >= 'a' && ch <= 'z') {
        return (ch -'a') * letter_hight + line;
    } else {
        return -1;
    }
}