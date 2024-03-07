#include "poke.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

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

char *get_abilities(char *text) {
    char *abilities;
    size_t index = get_substr_index(text, "abilities");
    if (index == SIZE_MAX)
      return NULL;
    size_t len = strlen(text);
    int i = 0;
    while (text[index + i] != ']' && (index + i) < len)
      i++;
    if (index + i < len)
      i++;
    abilities = malloc((i + 1) * sizeof(char));
    for (int j = 0; j < i; j++) {
      abilities[j] = text[index + j];
    }
    abilities[i] = '\0';
    return abilities;
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