#include "texture.h"
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
//void printMenu(WINDOW * menu, stdin choices[], int size, int highlight);

extern int generation(int longueur, int largeur, int num_salle, int cote);
extern unsigned int aleatoire(int salle, int graine, int min, int max);
extern void* play_music(void* arg);
extern int afficher_image_ascii(WINDOW *win, const char *filename);

extern int graine;
extern bool stop_music;

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
    ncurses_initialiser();
    ncurses_couleurs();
    ncurses_souris();

    raw(); // Utiliser raw pour obtenir les touches directement

    int start_y = 10;
    int start_x = 10;
    int x = 10;
    int y = 10;
    int ch;

    WINDOW *win = newwin(10, 20, start_y, start_x);
    refresh();

    /*
        COLOR_PAIR(n)
        COLOR_BLACK 0
        COLOR_RED 1
        COLOR_GREEN 2
        COLOR_YELLOW 3
        COLOR_BLUE 4
        COLOR_MAGENTA 5
        COLOR_CYAN 6
        COLOR_WHITE 7
    */

    init_pair(1, COLOR_CYAN, COLOR_WHITE);

    /*
    if (can_change_color()) {
        printw("on peut changer la couleur\n");
        init_color(COLOR_CYAN, 9, 999, 999);
    }
    attron(COLOR_PAIR(1));
    printw("texte de fou");
    attroff(COLOR_PAIR(1));

    int ch;
    ch = 0;
    // Boucle pour détecter l'appui sur la touche espace
    while ((ch = getch()) != ' ') {
        // Rien à faire, juste attendre l'appui sur espace
    }

    box(win, 0, 0); // Dessine le cadre de la fenêtre
    mvwprintw(win, 1, 1, "Chokbar de bz");
    wrefresh(win);

    win = newwin(10, 20, start_y + 0, start_x + 20);
    box(win, 0, 0); // Dessine le cadre de la fenêtre
    mvwprintw(win, 1, 1, "Chokbar de 2 bz");
    wrefresh(win);

    // Boucle pour détecter l'appui sur la touche espace
    ch = 0;
    while (ch != ' ') {
        ch = getch();
        // Rien à faire, juste attendre l'appui sur espace
    }

    //texte centré

    char *msg = "Texte au centre";
    int taille = strlen(msg);

    while (1) {
        clear(); // Efface le contenu de la fenêtre
        mvprintw(LINES / 2, (COLS / 2) - (taille / 2), "%s", msg);
        refresh();
        if (getch() != 410)
            break;
    }

    endwin();
    */

    WINDOW *menu_win;
    int highlight = 1;
    int choice = 0;
    int c = 0;

    int LINES = getmaxy(stdscr);
    int COLS = getmaxx(stdscr);

    int winHauteur = (int)(LINES * 0.3);
    int winLargeur = (int)(COLS * 0.6);

    int titleY = ((LINES - 5)/6);
    int titleX = (COLS - 69) / 2;

    int winY = ((LINES - winHauteur) / 4) * 3;
    int winX = (COLS - winLargeur) / 2;
    WINDOW *title = newwin(5, 69, titleY, titleX);

    if(afficher_image_ascii(title, "image/cosmicyonder.txt") != EXIT_SUCCESS){

        printf("Erreur");
        exit(EXIT_FAILURE);
    }
    wrefresh(title);

    menu_win = newwin(winHauteur, winLargeur, winY, winX);
    keypad(menu_win, TRUE);
    refresh();
    print_menu(menu_win, highlight, n_choices, choices);



    
    while (1) {
        // Check for terminal resize
        int new_LINES = getmaxy(stdscr);
        int new_COLS = getmaxx(stdscr);
        if (new_LINES != LINES || new_COLS != COLS) {
            LINES = new_LINES;
            COLS = new_COLS;

            winHauteur = (int)(LINES * 0.3);
            winLargeur = (int)(COLS * 0.6);

            winY = (LINES - winHauteur) / 2;
            winX = (COLS - winLargeur) / 2;
            titleY = ((LINES - 5)/6);
            titleX = (COLS - 69) / 2;

            clear();
            refresh();

            // Delete the old window and create a new one
            delwin(menu_win);
            menu_win = newwin(winHauteur, winLargeur, winY, winX);
            title = newwin(5, 69, titleY, titleX);
            if(afficher_image_ascii(title, "image/cosmicyonder.txt") != EXIT_SUCCESS){

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
                mvprintw(24, 0, "Character pressed is = %3d Hopefully it can be printed as '%c'", c, c);
                refresh();
                break;
        }
        if (choice != 0) // User did a choice come out of the infinite loop
            break;
    }

    mvprintw(23, 0, "You chose choice %d with choice string %s\n", choice, choices[choice - 1]);
    clrtoeol();
    refresh();
    endwin();

    switch (choice) {
        case 1:
            endwin();
            if(jeu() != EXIT_SUCCESS){
                printf("Erreur jeu");
                exit(EXIT_FAILURE);
            }

            endwin();

            return EXIT_SUCCESS;
            break;
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

            int tabLa, tabLo;

            printf("Valeur de la longueur puis de la largeur de la salle : \n");
            scanf("%d", &tabLa);

            // Libération de la mémoire de la première génération
            for (unsigned i = 0; i < 5; ++i) {
                free(room.cases[i]);
            }
            free(room.cases);
            room.cases = NULL; // Réinitialisation de room.cases pour éviter la double libération

            scanf("%d", &tabLo);

            if (generation(tabLa, tabLo, 5, 2) != EXIT_SUCCESS) {
                printf("Erreur generation salle\n");
                exit(EXIT_FAILURE);
            }

            for (unsigned i = 0; i < tabLa; ++i) {
                for (unsigned j = 0; j < tabLo; ++j) {
                    printf("[ %d ] ", room.cases[i][j].contenu);
                }
                printf("\n");
            }

            // Libération de la mémoire de la deuxième génération
            for (unsigned i = 0; i < tabLa; ++i) {
                free(room.cases[i]);
            }
            free(room.cases);
            room.cases = NULL;
            endwin();
            break;
            
        case 4:
            printf("debut musique ==================== \n");
            printf("Quelle musique ?");
            scanf("%d", &x);

            // Créer un thread pour jouer la musique
            pthread_t music_thread;
            pthread_create(&music_thread, NULL, play_music, &x);

            // Boucle pour détecter l'appui sur la touche espace
            ch = 0;
            while (ch != ' ') {
                ch = getch();
                // Rien à faire, juste attendre l'appui sur espace
            }

            // Arrêter la musique et attendre la fin du thread
            stop_music = true;
            pthread_join(music_thread, NULL);

            // Terminer ncurses
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
    // Boucle pour détecter l'appui sur la touche espace
    ch = 0;
    while (ch != ' ') {
        ch = getch();
        // Rien à faire, juste attendre l'appui sur espace
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
