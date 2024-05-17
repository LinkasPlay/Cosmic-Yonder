#include "texture.h"
#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <unistd.h>
#include <string.h>

extern int generation(int longueur, int largeur, int num_salle, int cote);

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

int main(void) {
    ncurses_initialiser();
    ncurses_couleurs();
    ncurses_souris();

    int start_y = 10;
    int start_x = 10;
    int x = 10;
    int y = 10;
    char *msg = "Texte au centre";
    int taille = strlen(msg);

    WINDOW *win = newwin(10, 20, start_y, start_x);
    refresh();

    box(win, 0, 0); // Dessine le cadre de la fenêtre
    mvwprintw(win, 1, 1, "Chokbar de bz");
    wrefresh(win);

    getch();

    while (1) {
        clear(); // Efface le contenu de la fenêtre
        mvprintw(LINES / 2, (COLS / 2) - (taille / 2), "%s", msg);
        refresh();
        if (getch() != 410)
            break;
    }

    if(generation(5, 5, 0, 0) != EXIT_SUCCESS){
        printf("Erreur generation salle");
    }

    endwin();

    for (unsigned i = 0; i < 5; ++i) {
        for (unsigned j = 0; j < 5; ++j) {
            printf("[ %d ] ",room.cases[i][j].contenu);
        }
        printf ("\n");
    }

    
    return 0;
}

 
    /*
    SDL_Surface *picture = NULL;
    SDL_Texture *texture = NULL;
    SDL_Rect dest_rect = {0,0,1147,480};


    if(SDL_Init(SDL_INIT_VIDEO) != 0){
        SDL_Log("Erreur : Initialisation SDL > %s\n", SDL_GetError());
        clean_ressources(NULL,NULL,NULL);
        exit(EXIT_FAILURE);
    }

    window = SDL_CreateWindow("Cosmic Yonder", SDL_WINDOWPOS_CENTERED,(SDL_WINDOWPOS_CENTERED),WINDOW_WIDTH,480,0);

    if(window == NULL){
        SDL_Log("Erreur : Creation fenetre echouee > %s\n", SDL_GetError());
        clean_ressources(NULL,NULL,NULL);
        exit(EXIT_FAILURE);
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    if(renderer == NULL){
        SDL_Log("Erreur > %s\n", SDL_GetError());
        clean_ressources(window,NULL,NULL);
        exit(EXIT_FAILURE);
    }

    picture = SDL_LoadBMP("src/image/spacet.bmp");
    if(picture == NULL){
        SDL_Log("Erreur > %s\n", SDL_GetError());
        clean_ressources(window,renderer,NULL);
        exit(EXIT_FAILURE);
    }

    texture = SDL_CreateTextureFromSurface(renderer, picture);
    SDL_FreeSurface(picture);
    if(texture == NULL){
        SDL_Log("Erreur > %s\n", SDL_GetError());
        clean_ressources(window,renderer,NULL);
        exit(EXIT_FAILURE);
    }

    if ((SDL_QueryTexture(texture, NULL, NULL, &dest_rect.w, &dest_rect.h)) != 0){
        SDL_Log("Erreur > %s\n", SDL_GetError());
        clean_ressources(window,renderer,texture);
        exit(EXIT_FAILURE);
    }

    if ((SDL_RenderCopy(renderer, texture, NULL, &dest_rect)) != 0){
        SDL_Log("Erreur > %s\n", SDL_GetError());
        clean_ressources(window,renderer,texture);
        exit(EXIT_FAILURE);
    }

    SDL_RenderPresent(renderer);

    clean_ressources(window, renderer, texture);

    SDL_bool program_launched = SDL_TRUE;

    while(program_launched){
        
        SDL_Event event;

        while (SDL_PollEvent(&event)){
            switch(event.type){
                case SDL_QUIT:
                    program_launched = SDL_FALSE;
                    break;
                
                default:
                    break;
            }
        }
        

    }



    


    return EXIT_SUCCESS;
}
*/

    
    
