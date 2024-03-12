#include <curses.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/easy.h>
#include <ncurses.h>
#include <unistd.h>

#include "curl_utils.h"
#include "poke.h"

const char* letters[] = {"     ",
                      ".d88 ",
                      "8  8 ",
                      "`Y88 ",
                      "     ",
                      "8    ",
                      "88b. ",
                      "8  8 ",
                      "88P' ",
                      "     ",
                      "     ",
                      ".d8b ",
                      "8    ",
                      "`Y8P ",
                      "     ",
                      "   8 ",
                      ".d88 ",
                      "8  8 ",
                      "`Y88 ",
                      "     ",
                      "      ",
                      ".d88b ",
                      "8.dP' ",
                      "`Y88P ",
                      "     ",
                      " d8b ",
                      " 8'  ",
                      "w8ww ",
                      " 8   ",
                      "     ",
                      "     ",
                      ".d88 ",
                      "8  8 ",
                      "`Y88 ",
                      "wwdP ",
                      "8     ",
                      "8d8b. ",
                      "8P Y8 ",
                      "8   8 ",
                      "      ",
                      "w ",
                      "w ",
                      "8 ",
                      "8 ",
                      "  ",
                      "  w ",
                      "  w ",
                      "  8 ",
                      "  8 ",
                      "wdP ",
                      "8    ",
                      "8.dP ",
                      "88b  ",
                      "8 Yb ",
                      "     ",
                      "8 ",
                      "8 ",
                      "8 ",
                      "8 ",
                      "  ",
                      "          ",
                      "8d8b.d8b. ",
                      "8P Y8P Y8 ",
                      "8   8   8 ",
                      "          ",
                      "      ",
                      "8d8b. ",
                      "8P Y8 ",
                      "8   8 ",
                      "      ",
                      "      ",
                      ".d8b. ",
                      "8' .8 ",
                      "`Y8P' ",
                      "      ",
                      "     ",
                      "88b. ",
                      "8  8 ",
                      "88P' ",
                      "8    ",
                      "      ",
                      ".d88  ",
                      "8  8  ",
                      "`Y88  ",
                      "   8P ",
                      "     ",
                      "8d8b ",
                      "8P   ",
                      "8    ",
                      "     ",
                      "     ",
                      "d88b ",
                      "`Yb. ",
                      "Y88P ",
                      "     ",
                      " w   ",
                      "w8ww ",
                      " 8   ",
                      " Y8P ",
                      "     ",
                      "      ",
                      "8   8 ",
                      "8b d8 ",
                      "`Y8P8 ",
                      "      ",
                      "       ",
                      "Yb  dP ",
                      " YbdP  ",
                      "  YP   ",
                      "       ",
                      "           ",
                      "Yb  db  dP ",
                      " YbdPYbdP  ",
                      "  YP  YP   ",
                      "           ",
                      "      ",
                      "Yb dP ",
                      " `8.  ",
                      "dP Yb ",
                      "      ",
                      "       ",
                      "Yb  dP ",
                      " YbdP  ",
                      "  dP   ",
                      " dP    ",
                      "     ",
                      "888P ",
                      " dP  ",
                      "d888 ",
                      "     "};

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

    if (response_data.data == NULL) {
        fprintf(stderr, "Could not find %s\n",argv[1]);
        return EXIT_FAILURE;
    }

    Poke_data poke; 
    get_poke_data(&poke, response_data.data);

    initscr();
    cbreak();
    nodelay(stdscr, TRUE);
    noecho();
    curs_set(0);

    int height = LINES - 10;
    int width = 30;
    int starty = LINES - height;
    int startx = (COLS - width) / 7;

    WINDOW* win_b = newwin(height, width, starty, 0);
    WINDOW* win2_b = newwin(height, width, starty, startx+2);
    WINDOW* win3_b = newwin(height, width, starty, startx*2+4);
    WINDOW* win4_b = newwin(height, width, starty, startx*3+6);
    WINDOW* win5_b = newwin(height, width, starty, startx*4+8);
    WINDOW* win6_b = newwin(height, width, starty, startx*5+10);
    WINDOW* win7_b = newwin(height, width, starty, startx*6+12);

    WINDOW* title = newwin(10, COLS/2, 0, 0);

    int letter_line = 0;
    int letter_len = 0;
    
    char ch;
    while (1) {

        for (int i = 0; i<5; i++) {
            for (int j = 0; argv[1][j] != '\0'; j++) {
                letter_line = cool_name(argv[1][j],5,i);
                mvwprintw(title,2+i, 2+letter_len, "%s", letters[letter_line]);                
                letter_len += (int)strlen(letters[letter_line]);
            }
            letter_len = 0;
        }
        box(title, 0, 0);
        wrefresh(title);

        box(win_b , 0, 0);
        box(win2_b, 0, 0);
        box(win3_b, 0, 0);
        box(win4_b, 0, 0);
        box(win5_b, 0, 0);
        box(win6_b, 0, 0);
        box(win7_b, 0, 0);
        mvwprintw(win_b, 1, 1, "%s",poke.abs);
        mvwprintw(win2_b, 1, 1, "%s",poke.forms);
        mvwprintw(win3_b, 1, 1, "%s",poke.held_items);
        mvwprintw(win4_b, 1, 1, "%s",poke.game_indices);
        mvwprintw(win5_b, 1, 1, "%s",poke.moves);
        mvwprintw(win6_b, 1, 1, "%s",poke.stats);
        mvwprintw(win7_b, 1, 1, "%s",poke.types);
        wrefresh(win_b);
        wrefresh(win2_b);
        wrefresh(win3_b);
        wrefresh(win4_b);
        wrefresh(win5_b);
        wrefresh(win6_b);
        wrefresh(win7_b);
        if ((ch = getch()) != ERR) {
            if (ch == 'q') break;
        }
        usleep(1000000);
    }

    delwin(win7_b);
    delwin(win6_b);
    delwin(win5_b);
    delwin(win4_b);
    delwin(win3_b);
    delwin(win2_b);
    delwin(win_b);
    delwin(title);
    endwin();

    clean_poke_data(&poke);
    curl_easy_cleanup(curl_h);
    return EXIT_SUCCESS;
}