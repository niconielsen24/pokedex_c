#include "poke.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
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

static int key_len(char c){
    uint8_t letter_width_0 = 10; //z,y,v,u,s,r,q,p,o,n,k,h,g,f,e,d,c,b,a
    uint8_t letter_width_1 = 12; //w,m
    uint8_t letter_width_2 = 8;  //t
    uint8_t letter_width_3 = 4;  //i,j,l

    if (
        c == 'a' || c == 'b' || c == 'c' || c == 'd' || c == 'e' || c == 'f' ||
        c == 'g' || c == 'g' || c == 'h' || c == 'g' || c == 'h' || c == 'k' ||
        c == 'n' || c == 'o' || c == 'p' || c == 'y' || c == 'z' || c == 'v' ||
        c == 'q' || c == 'r' || c == 's' || c == 'u'
        ) {return letter_width_0;}
    else if (
        c == 'w' || c == 'm'
    ) {return letter_width_1;}
    else if (
        c == 't'
    ) {return letter_width_2;}
    else if (
        c == 'i' || c == 'j' || c == 'l'
    ) {return letter_width_3;}

    return -1;
}

void print_letter_line(char ch, int line){
    char* a[] = {"         ", 
                 "         ",
                 ".d8888b. ",
                 "88'  `88 ", 
                 "88.  .88 ", 
                 "`88888P8 ",
                 "         ",
                 "         "};
    
    char* b[] =  {"dP       ",
                  "88       ",
                  "88d888b. ",
                  "88'  `88 ",
                  "88.  .88 ",
                  "88Y8888' ",
                  "         ",
                  "         "};
    
    char* c[] =  {"         ",
                  "         ",
                  ".d8888b. ",
                  "88'  ``` ",
                  "88.  ... ",
                  "`88888P' ",
                  "         ",
                  "         "};
    
    char* d[] =  {"      dP ",
                  "      88 ",
                  ".d888b88 ",
                  "88'  `88 ",
                  "88.  .88 ",
                  "`88888P8 ",
                  "         ",
                  "         "};
    
    char* e[] =  {"         ",
                 "         ",
                 ".d8888b. ",
                 "88ooood8 ",
                 "88.  ... ",
                 "`88888P' ",
                 "         ",
                 "         "};
    
    char* f[] =  {".8888b   ",
                 "88       ",
                 "88aaa    ",
                 "88       ",
                 "88       ",
                 "dP       ",
                 "         ",
                 "         "};
    
    char* g[] =  {"         ",
                 "         ",
                 ".d8888b. ",
                 "88'  `88 ",
                 "88.  .88 ",
                 "`8888P88 ",
                 "     .88 ",
                 " d8888P  "};
       
    char* h[] =  {"dP       ",
                 "88       ",
                 "88d888b. ",
                 "88'  `88 ",
                 "88    88 ",
                 "dP    dP ",
                 "         ",
                 "         "};
         
    char* i[] =  {"oo ",
                 "   ",
                 "dP ",
                 "88 ",
                 "88 ",
                 "dP ",
                 "   ",
                 "   "};
   
    char* j[] =  {"oo ",
                 "   ",
                 "dP ",
                 "88 ",
                 "88 ",
                 "88 ",
                 "88 ",
                 "dP "};

    char* k[] =  {"dP       ",
                 "88       ",
                 "88  .dP  ",
                 "88888'   ",
                 "88  `8b. ",
                 "dP   `YP ",
                 "         ",
                 "         "};
    
    char* l[] =  {"dP ",
                 "88 ",
                 "88 ",
                 "88 ",
                 "88 ",
                 "dP ",
                 "   ",
                 "   "};

    char* m[] =  {"           ", 
                 "           ",
                 "88d8b.d8b. ",
                 "88'`88'`88 ",
                 "88  88  88 ",
                 "dP  dP  dP ",
                 "           ",
                 "           "};

    char* n[] =  {"         ",
                 "         ",
                 "88d888b. ",
                 "88'  `88 ",
                 "88    88 ",
                 "dP    dP ",
                 "         ",
                 "         "};
        
    char* o[] =  {"         ",
                 "         ",
                 ".d8888b. ",
                 "88'  `88 ",
                 "88.  .88 ",
                 "`88888P' ",
                 "         ",
                 "         "};

    char* p[] =  {"         ",
                 "         ",
                 "88d888b. ",
                 "88'  `88 ",
                 "88.  .88 ",
                 "88Y888P' ",
                 "88       ",
                 "dP       "};

    char* q[] =  {"         ",
                 "         ",
                 ".d8888b. ",
                 "88'  `88 ",
                 "88.  .88 ",
                 "`8888P88 ",
                 "      88 ",
                 "      dP "};
    
    char* r[] =  {"         ",
                 "         ",
                 "88d888b. ",
                 "88'  `88 ",
                 "88       ",
                 "dP       ",
                 "         ",
                 "         "};

    char* s[] =  {"         ",
                 "         ",
                 ".d8888b. ",
                 "Y8ooooo. ",
                 "      88 ",
                 "`88888P' ",
                 "         ",
                 "         "};
    
    char* t[] = {"  dP   ",
                "  88   ",
                "d8888P ",
                "  88   ",
                "  88   ",
                "  dP   ",
                "       ",
                "       "};
    
    char* u[] = {"         ",
                "         ",
                "dP    dP ",
                "88    88 ",
                "88.  .88 ",
                "`88888P' ",
                "         ",
                "         "};

    char* v[] = {"         ",
                "         ",
                "dP   .dP ",
                "88   d8' ",
                "88 .88'  ",
                "8888P'   ",
                "         ",
                "         "};

    char* w[] = {"           ",
                "           ",
                "dP  dP  dP ",
                "88  88  88 ",
                "88.88b.88' ",
                "8888P Y8P  ",
                "           ",
                "           "};
    
    char* x[] = {"         ",
                "         ",
                "dP.  .dP ",
                " `8bd8'  ",
                " .d88b.  ",
                "dP'  `dP ",
                "         ",
                "         "};
    
    char* y[] = {"         ",
                "         ",
                "dP    dP ",
                "88    88 ",
                "88.  .88 ",
                "`8888P88 ",
                "     .88 ",
                " d8888P  "};

    char* z[] = {"         ",
                "         ",
                "d888888b ",
                "   .d8P' ",
                " .Y8P    ",
                "d888888P ",
                "         ",
                "         "};

    switch (ch) {
        case 'a':
                printf("%s",a[line]);
                break;
        case 'b':
                printf("%s",b[line]);
                break;
        case 'c':
                printf("%s",c[line]);
                break;
        case 'd':
                printf("%s",d[line]);
                break;
        case 'e':
                printf("%s",e[line]);
                break;
        case 'f':
                printf("%s",f[line]);
                break;
        case 'g':
                printf("%s",g[line]);
                break;
        case 'h':
                printf("%s",h[line]);
                break;
        case 'i':
                printf("%s",i[line]);
                break;
        case 'j':
                printf("%s",j[line]);
                break;
        case 'k':
                printf("%s",k[line]);
                break;
        case 'l':
                printf("%s",l[line]);
                break;
        case 'm':
                printf("%s",m[line]);
                break;
        case 'n':
                printf("%s",n[line]);
                break;
        case 'o':
                printf("%s",o[line]);
                break;
        case 'p':
                printf("%s",p[line]);
                break;
        case 'q':
                printf("%s",q[line]);
                break;
        case 'r':
                printf("%s",r[line]);
                break;
        case 's':
                printf("%s",s[line]);
                break;
        case 't':
                printf("%s",t[line]);
                break;
        case 'u':
                printf("%s",u[line]);
                break;
        case 'v':
                printf("%s",v[line]);
                break;
        case 'w':
                printf("%s",w[line]);
                break;
        case 'x':
                printf("%s",x[line]);
                break;
        case 'y':
                printf("%s",y[line]);
                break;
        case 'z':
                printf("%s",z[line]);
                break;
        default:
                printf("Invalid choice\n");
                break;
    }
}

void name(char* name){

    for (int i = 0; i<8 ; i++) {
        for (int j = 0; name[j] != '\0'; j++) {
            print_letter_line(name[j],i);
        }
        printf("\n");
    }    
}