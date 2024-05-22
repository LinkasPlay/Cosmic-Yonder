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

tile contenuCase;

extern personnage perso;

int camera(void);

extern int mouvementHaut(void);
extern int mouvementGauche(void);
extern int mouvementBas(void);
extern int mouvementDroite(void);
extern int attaqueEpee(void);

extern int texture(WINDOW *win);
extern int creeMap(void);
extern int actualiserMap(void);

int jeu (void) {

    int LINES = getmaxy(stdscr);
    int COLS = getmaxx(stdscr);

    int winHauteur = (int)(LINES * 0.8);
    int winLargeur = (int)(COLS * 0.8);
    int winY = (LINES - winHauteur) / 2;
    int winX = (COLS - winLargeur) / 2;

    if (perso.lvl == 0) {
        if (creeMap() != 0) {
            printf("Impossible de créer la map\n");
            endwin();
            return 1;
        }
    }

    WINDOW *win = newwin(winHauteur, winLargeur, winY, winX);
    box(win, 0, 0); // Dessine le cadre de la fenêtre
    mvwprintw(win, 1, 1, "Chokbar de 2 bz");
    wrefresh(win);

    // Boucle pour détecter l'appui sur la touche espace
    int ch = 0;
    while (ch != ' ') {
        ch = getch();
        // Rien à faire, juste attendre l'appui sur espace
    }

    // Texte centré
    char *msg = "CHOKBAR DE GIGA BAISé";
    int taille = strlen(msg);

    while (1) {
        // Mettre à jour les dimensions de l'écran
        LINES = getmaxy(stdscr);
        COLS = getmaxx(stdscr);

        winHauteur = (int)(LINES * 0.8);
        winLargeur = (int)(COLS * 0.8);
        winY = (LINES - winHauteur) / 2;
        winX = (COLS - winLargeur) / 2;

        clear(); // Efface le contenu de la fenêtre principale
        refresh(); // Rafraîchit la fenêtre principale

        // Supprime l'ancienne fenêtre avant d'en créer une nouvelle
        delwin(win);
        
        win = newwin(winHauteur, winLargeur, winY, winX);
        box(win, 0, 0); // Dessine le cadre de la fenêtre
        mvwprintw(win, winHauteur / 2, (winLargeur / 2) - (taille / 2), "%s", msg);
        wrefresh(win);

        if (getch() == ' ') // Touche pour sortir
            break;
    }

    endwin();
	
    return EXIT_SUCCESS;

	if (camera() != EXIT_SUCCESS){
		printf("Probleme fonction camera");
		endwin();
		exit(EXIT_FAILURE);
	}

	
	
	// Boucle de jeu ******************************************************************************** /

	printf("fini");
	unsigned int frame_limit = 0;

	//gestion des évènements

	ch = 0;

	while (ch != KEY_EXIT) {
		ch = getch();
        switch (ch) {
            case KEY_EXIT: /*Pour quitter le jeu*/
                return 1;
				break;

			//attaque
			case 'e':
            case ' ':
				if(attaqueEpee() != EXIT_SUCCESS){
					printf("Erreur attaque");
				}
				continue;

			//mvouvement haut
			case 'z':
			case KEY_UP:
			if(mouvementHaut() != EXIT_SUCCESS){
				printf("Erreur mouvement haut");
			}
			continue;
            
			//mvouvement gauche
			case 'q':
			case KEY_LEFT:
			if(mouvementGauche() != EXIT_SUCCESS){
				printf("Erreur mouvement gauche");
			}
			continue;

			//mvouvement bas
			case 's':
			case KEY_DOWN:
			if(mouvementBas() != EXIT_SUCCESS){
				printf("Erreur mouvement bas");
			}
			continue;

			//mvouvement droite
			case 'd':
			case KEY_RIGHT:
			if(mouvementDroite() != EXIT_SUCCESS){
				printf("Erreur mouvement droite");
			}
			continue;
        }
    }

	endwin();

	return EXIT_SUCCESS;
}

// afficher les images de la camera
int camera(void){

	int Xcase,Ycase;

	for ( Xcase = 1 ; Xcase < ( (WINDOW_WIDTH / 100) - 1) ; Xcase ++ ) {
		
        for ( Ycase = 1 ; Ycase < ( (WINDOW_HEIGHT / 100) - 1) ; Ycase ++ ) {

			printf("%d\n",(Xcamera + Xcase));
			contenuCase = map[Xcamera + Xcase][Ycamera + Ycase];
			//contenuCase = -5;
			//printf("%d",contenuCase);
            //Case.x = Xcase * 100;
            //Case.y = Ycase * 100;
            //Case.w = WINDOW_WIDTH / (WINDOW_WIDTH / 100) ;
	        //Case.h = WINDOW_HEIGHT / (WINDOW_HEIGHT / 100) ;
			WINDOW *win = newwin(5, 10, Ycamera, Xcase);
   	 		box(win, 0, 0); // Dessine le cadre de la fenêtre
    		wrefresh(win);
			if(texture(win) != EXIT_SUCCESS){
				printf("Fonction texture interompue");
				return EXIT_FAILURE;
			}
			
        }
    }
	perso.frameAnimation ++;
	return EXIT_SUCCESS;


}

