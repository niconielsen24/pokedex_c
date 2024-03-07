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

    int height = LINES - 5;
    int width = 30;
    int starty = (LINES - height) / 7;
    int startx = (COLS - width) / 7;

    WINDOW* win_b = newwin(height, width, starty, 0);
    WINDOW* win_w = newwin(height-2, width-2, starty+1, 1);
    WINDOW* win2_b = newwin(height, width, starty, startx+2);
    WINDOW* win2_w = newwin(height-2, width-2, starty+1, startx+3);
    WINDOW* win3_b = newwin(height, width, starty, startx*2+4);
    WINDOW* win3_w = newwin(height-2, width-2, starty+1, startx*2+5);
    WINDOW* win4_b = newwin(height, width, starty, startx*3+6);
    WINDOW* win4_w = newwin(height-2, width-2, starty+1, startx*3+7);
    WINDOW* win5_b = newwin(height, width, starty, startx*4+8);
    WINDOW* win5_w = newwin(height-2, width-2, starty+1, startx*4+9);
    WINDOW* win6_b = newwin(height, width, starty, startx*5+10);
    WINDOW* win6_w = newwin(height-2, width-2, starty+1, startx*5+11);
    WINDOW* win7_b = newwin(height, width, starty, startx*6+12);
    WINDOW* win7_w = newwin(height-2, width-2, starty+1, startx*6+13);

    char ch;
    while (1) {
        box(win_b , 0, 0);
        box(win2_b, 0, 0);
        box(win3_b, 0, 0);
        box(win4_b, 0, 0);
        box(win5_b, 0, 0);
        box(win6_b, 0, 0);
        box(win7_b, 0, 0);
        mvwprintw(win_w , 0, 0, "%s",poke.abs);
        mvwprintw(win2_w, 0, 0, "%s",poke.forms);
        mvwprintw(win3_w, 0, 0, "%s",poke.held_items);
        mvwprintw(win4_w, 0, 0, "%s",poke.game_indices);
        mvwprintw(win5_w, 0, 0, "%s",poke.moves);
        mvwprintw(win6_w, 0, 0, "%s",poke.stats);
        mvwprintw(win7_w, 0, 0, "%s",poke.types);
        wrefresh(win_b);
        wrefresh(win2_b);
        wrefresh(win3_b);
        wrefresh(win4_b);
        wrefresh(win5_b);
        wrefresh(win6_b);
        wrefresh(win7_b);
        wrefresh(win_w);
        wrefresh(win2_w);
        wrefresh(win3_w);
        wrefresh(win4_w);
        wrefresh(win5_w);
        wrefresh(win6_w);
        wrefresh(win7_w);
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
    delwin(win7_w);
    delwin(win6_w);
    delwin(win5_w);
    delwin(win4_w);
    delwin(win3_w);
    delwin(win2_w);
    delwin(win_w);
    endwin();

    clean_poke_data(&poke);
    curl_easy_cleanup(curl_h);

    name(argv[1]);

    return EXIT_SUCCESS;
}