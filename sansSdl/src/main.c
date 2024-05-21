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

extern int graine;
extern bool stop_music;

salle room;

int L, C; /*L pour désigner la ligne et C la colonne du click de la souris*/

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

int main(int argc, char ** argv){
    ncurses_initialiser();
    ncurses_couleurs();
    ncurses_souris();

    raw(); // Utiliser raw pour obtenir les touches directement

    int start_y = 10;
    int start_x = 10;
    int x = 10;
    int y = 10;
    char *msg = "Texte au centre";
    int taille = strlen(msg);

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

    if (can_change_color()) {
        printw("on peut changer la couleur\n");
        init_color(COLOR_CYAN, 9, 999, 999);
    }
    attron(COLOR_PAIR(1));
    printw("texte de fou");
    attroff(COLOR_PAIR(1));

    int ch;
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

    while (1) {
        clear(); // Efface le contenu de la fenêtre
        mvprintw(LINES / 2, (COLS / 2) - (taille / 2), "%s", msg);
        refresh();
        if (getch() != 410)
            break;
    }

    // Taille écran
    int yMax, xMax;
    getmaxyx(stdscr, yMax, xMax);
    // Création fenêtre
    WINDOW *inputwin = newwin(6, xMax - 12, yMax - 8, 5);
    box(inputwin, 0, 0);
    refresh();
    wrefresh(inputwin);
    keypad(inputwin, true);

    // Boucle pour détecter l'appui sur la touche espace
    ch = 0;
    while (ch != ' ') {
        ch = getch();
        // Rien à faire, juste attendre l'appui sur espace
    }

    endwin();

    if (generation(5, 5, 1, 0) != EXIT_SUCCESS) {
        printf("Erreur generation salle\n");
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
    }

    for (unsigned i = 0; i < tabLa; ++i) {
        for (unsigned j = 0; j < tabLo; ++j) {
            printf("[ %d ] ", room.cases[i][j].contenu);
        }
        printf("\n");
    }

    /*
    do {
        printf(" \n\n Valeur min et max de al : \n");
        scanf("%d", &tabLa);
        scanf("%d", &tabLo);
        printf("\nal = %d \n", aleatoire(3, graine, tabLa, tabLo));
        scanf("%d", &x);
    } while (x!=0);

    for(unsigned z = 0; z<20; z++){
        printf("al %d = %d \n",z, aleatoire(z * 5, graine * z - 5, 1, 100));
        printf("al2 %d = %d \n\n",z, aleatoire(z * 5, graine * z - 5, 1, 6));
    }
    */

    // Libération de la mémoire de la deuxième génération
    for (unsigned i = 0; i < tabLa; ++i) {
        free(room.cases[i]);
    }
    free(room.cases);
    room.cases = NULL;

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

    return 0;
}