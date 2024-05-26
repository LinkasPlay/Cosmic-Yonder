#include "CosmicYonder.h"
#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <pulse/simple.h>
#include <pulse/error.h>

// COMMANDE TERMINAL : gcc -o ProgMain *.c -lncurses -lm -lpthread -lpulse-simple -lpulse

void start_ncurses(bool useRaw, bool useNoecho);


extern int generation(int longueur, int largeur, int num_salle, int cote);
extern unsigned int aleatoire(int salle, int graine, int min, int max);
extern void* play_music(void* arg);

extern int afficher_image_ascii(WINDOW *win, const char *filename);

extern int liberationMap(void);

int graine;
extern bool stop_music;
extern personnage perso;

salle room;

int L, C; /*L pour désigner la ligne et C la colonne du click de la souris*/

void print_menu(WINDOW *menu_win, int highlight, int n_choices, char *choices[]);
void get_player_info(void);


char *choices[] = { 
    "Nouvelle partie",
    "Charger partie",
    "Option",
    "Soundtrack",
    "Quitter le jeu",
};

int n_choices = 5;

void ncurses_initialiser() {
    initscr();          /* Démarre le mode ncurses */
    cbreak();           /* Pour les saisies clavier (désactive la mise en buffer) */
    noecho();           /* Désactive l'affichage des caractères saisis */
    keypad(stdscr, TRUE);   /* Active les touches spécifiques */
    refresh();          /* Met à jour l'affichage */
    curs_set(FALSE);    /* Masque le curseur */
}

void ncurses_couleurs() {
    if (has_colors() == FALSE) {
        endwin();
        fprintf(stderr, "Le terminal ne supporte pas les couleurs.\n");
        exit(EXIT_FAILURE);
    }
    start_color();
}

void ncurses_souris() {
    if (!mousemask(ALL_MOUSE_EVENTS, NULL)) {
        endwin();
        fprintf(stderr, "Erreur lors de l'initialisation de la souris.\n");
        exit(EXIT_FAILURE);
    }
    if (has_mouse() != TRUE) {
        endwin();
        fprintf(stderr, "Aucune souris n'est détectée.\n");
        exit(EXIT_FAILURE);
    }
}

int click_souris() {
    MEVENT event;
    int ch;
    while ((ch = getch()) != KEY_F(1)) {
        switch (ch) {
            case KEY_F(2): /*Pour quitter le jeu*/
                return 1;
            case KEY_MOUSE:
                if (getmouse(&event) == OK) {
                    C = event.x;
                    L = event.y;
                    if (event.bstate & BUTTON1_CLICKED) {
                        if (L < 51 && C < 132) {
                            return 0;
                        }
                    }
                }
        }
    }
    return 0;
}


int main(int argc, char **argv) {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    refresh();
    curs_set(FALSE);

    if (has_colors()) {
        start_color();
    }

    raw();

    int start_y = 10;
    int start_x = 10;
    int x = 10;
    int y = 10;
    int ch;

    WINDOW *win = newwin(10, 20, start_y, start_x);
    refresh();

    init_pair(1, COLOR_CYAN, COLOR_WHITE);

    char *choices[] = { 
        "Nouvelle partie",
        "Charger partie",
        "Option",
        "Soundtrack",
        "Quitter le jeu",
    };
    int n_choices = sizeof(choices) / sizeof(choices[0]);
    int highlight = 1;
    int choice = 0;
    int c = 0;

    int LINES = getmaxy(stdscr);
    int COLS = getmaxx(stdscr);

    int winHauteur = (int)(LINES * 0.3);
    int winLargeur = (int)(COLS * 0.6);

    int titleY = ((LINES - 5) / 6);
    int titleX = (COLS - 69) / 2;

    int winY = ((LINES - winHauteur) / 4) * 3;
    int winX = (COLS - winLargeur) / 2;
    WINDOW *title = newwin(5, 69, titleY, titleX);

    if (afficher_image_ascii(title, "image/cosmicyonder.txt") != EXIT_SUCCESS) {
        printf("Erreur");
        exit(EXIT_FAILURE);
    }
    wrefresh(title);

    WINDOW *menu_win = newwin(winHauteur, winLargeur, winY, winX);
    keypad(menu_win, TRUE);
    refresh();
    print_menu(menu_win, highlight, n_choices, choices);

    pthread_t music_th;
    int music_choice = 0;
    pthread_create(&music_th, NULL, play_music, &music_choice);

    while (1) {
        int new_LINES = getmaxy(stdscr);
        int new_COLS = getmaxx(stdscr);
        if (new_LINES != LINES || new_COLS != COLS) {
            LINES = new_LINES;
            COLS = new_COLS;

            winHauteur = (int)(LINES * 0.3);
            winLargeur = (int)(COLS * 0.6);

            winY = (LINES - winHauteur) / 2;
            winX = (COLS - winLargeur) / 2;
            titleY = ((LINES - 5) / 6);
            titleX = (COLS - 69) / 2;

            clear();
            refresh();

            delwin(menu_win);
            menu_win = newwin(winHauteur, winLargeur, winY, winX);
            delwin(title);
            title = newwin(5, 69, titleY, titleX);
            if (afficher_image_ascii(title, "image/cosmicyonder.txt") != EXIT_SUCCESS) {
                printf("Erreur");
                exit(EXIT_FAILURE);
            }
            wrefresh(title);
            keypad(menu_win, TRUE);
            refresh();
        }

        print_menu(menu_win, highlight, n_choices, choices);
        c = wgetch(menu_win);
        switch (c) {
            case KEY_UP:
                if (highlight == 1)
                    highlight = n_choices;
                else
                    --highlight;
                break;
            case KEY_DOWN:
                if (highlight == n_choices)
                    highlight = 1;
                else
                    ++highlight;
                break;
            case 10: // Enter key
                choice = highlight;
                break;
            default:
                refresh();
                break;
        }
        if (choice != 0) // User made a choice, break out of the loop
            break;
    }

    stop_music = true;
    pthread_join(music_th, NULL);

    clrtoeol();
    refresh();
    endwin();

    switch (choice) {
        case 1:
            perso.lvl = 0;
            char player_name[50];
            int player_seed;
            get_player_info();
            clear();
            mvprintw(0, 0, "Player Name: %s", player_name);
            mvprintw(1, 0, "Seed: %d", player_seed);
            refresh();

            if (jeu() != EXIT_SUCCESS) {
                printf("Erreur jeu");
                exit(EXIT_FAILURE);
            }

            if (liberationMap() != EXIT_SUCCESS) {
                printf("Erreur liberation map");
                exit(EXIT_FAILURE);
            }

            endwin();
            return EXIT_SUCCESS;
            
        case 2:
            if (generation(5, 5, 1, 0) != EXIT_SUCCESS) {
                printf("Erreur generation salle\n");
                exit(EXIT_FAILURE);
            }

            for (unsigned i = 0; i < 5; ++i) {
                for (unsigned j = 0; j < 5; ++j) {
                    printf("[ %d ] ", room.cases[i][j].contenu);
                }
                printf("\n");
            }

            for (unsigned i = 0; i < 5; ++i) {
                free(room.cases[i]);
            }
            free(room.cases);
            room.cases = NULL;
            endwin();
            break;
        case 3:
            printf("Options\n");
            break;
        case 4:
            printf("debut musique ==================== \n");
            printf("Quelle musique ?");
            scanf("%d", &x);

            pthread_t music_thread;
            pthread_create(&music_thread, NULL, play_music, &x);

            ch = 0;
            while (ch != 10) { // Enter key
                ch = getch();
            }

            stop_music = true;
            pthread_join(music_thread, NULL);

            endwin();
            break;
        case 5:
            printf("Fin du jeu\n");
            endwin();
            exit(EXIT_SUCCESS);
            break;
        default:
            break;
    }

    ch = 0;
    while (ch != ' ') {
        ch = getch();
    }
    

    endwin();

    return EXIT_SUCCESS;
}

void print_menu(WINDOW *menu_win, int highlight, int n_choices, char *choices[]) {
    int x, y, i;
    x = 2;
    y = 2;
    box(menu_win, 0, 0);
    for (i = 0; i < n_choices; ++i) {
        if (highlight == i + 1) { // Highlight the present choice
            wattron(menu_win, A_REVERSE); 
            mvwprintw(menu_win, y, x, "%s", choices[i]);
            wattroff(menu_win, A_REVERSE);
        } else
            mvwprintw(menu_win, y, x, "%s", choices[i]);
        ++y;
    }
    wrefresh(menu_win);
}

bool is_alpha(int ch) {
    return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z');
}

bool is_digit(int ch) {
    return ch >= '0' && ch <= '9';
}

void get_player_info() {
    int height = 10, width = 50;
    int start_y = (LINES - height) / 2;
    int start_x = (COLS - width) / 2;
    int player_seed;
    char player_name[8]; // 7 characters + null terminator

    WINDOW *info_win = newwin(height, width, start_y, start_x);
    box(info_win, 0, 0);
    mvwprintw(info_win, 1, 2, "Enter your name (max 7 characters): ");
    mvwprintw(info_win, 3, 2, "Enter a seed (numbers only, max 7 digits): ");

    char name_input[8] = {0}; // 7 characters + null terminator
    char seed_input[8] = {0}; // 7 digits + null terminator
    int name_index = 0, seed_index = 0;
    bool entering_name = true;
    int ch;

    echo();
    keypad(info_win, TRUE);

    while (1) {
        if (entering_name) {
            wmove(info_win, 1, 35 + name_index);
        } else {
            wmove(info_win, 3, 32 + seed_index);
        }
        wrefresh(info_win);

        ch = wgetch(info_win);
        if (ch == '\t') {
            entering_name = !entering_name;
            continue;
        } else if (ch == '\n') {
            if (!entering_name && seed_index > 0)
                break;
            else
                continue;
        }

        if (entering_name && name_index < 7 && ch != ':' && name_index > 0) {
            name_input[name_index++] = ch;
            mvwaddch(info_win, 1, 35 + name_index - 1, ch);
        } else if (!entering_name && seed_index < 7 && is_digit(ch)) {
            seed_input[seed_index++] = ch;
            mvwaddch(info_win, 3, 32 + seed_index - 1, ch);
        }

        wrefresh(info_win);
    }

    noecho();

    player_seed = atoi(seed_input);

    strncpy(player_name, name_input, sizeof(player_name) - 1);
    player_name[sizeof(player_name) - 1] = '\0';

    wrefresh(info_win);
    delwin(info_win);
}








//test