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
void demander_nom_et_graine(void);
//void printMenu(WINDOW * menu, stdin choices[], int size, int highlight);

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
            demander_nom_et_graine();

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

void demander_nom_et_graine(void) {
    // Taille maximale du nom
    const int nom_max_len = 50;

    int LINES = getmaxy(stdscr);
    int COLS = getmaxx(stdscr);

    // Calculer les positions de la fenêtre et de l'entrée
    int winHauteur = 10;
    int winLargeur = 50;
    int winY = (LINES - winHauteur) / 2;
    int winX = (COLS - winLargeur) / 2;

    WINDOW *input_win = newwin(winHauteur, winLargeur, winY, winX);
    box(input_win, 0, 0);
    mvwprintw(input_win, 1, 1, "Entrez votre nom:");
    mvwprintw(input_win, 3, 1, "Entrez une graine:");

    // Afficher la fenêtre
    wrefresh(input_win);

    // Lire le nom du joueur
    char nom[nom_max_len + 1];
    mvwgetnstr(input_win, 1, 18, nom, nom_max_len); // Position après "Entrez votre nom:"
    nom[nom_max_len] = '\0'; // S'assurer que la chaîne est terminée par un '\0'

    // Lire la graine
    char graine_str[11]; // Espace suffisant pour un entier et le '\0'
    mvwgetnstr(input_win, 3, 18, graine_str, 10); // Position après "Entrez une graine:"
    graine_str[10] = '\0'; // S'assurer que la chaîne est terminée par un '\0'
    int graine = atoi(graine_str);

    // Nettoyer la fenêtre
    delwin(input_win);

    // Terminer ncurses
    endwin();

    // Afficher les valeurs (pour test)
    printf("Nom: %s\n", nom);
    printf("Graine: %d\n", graine);
}

//test