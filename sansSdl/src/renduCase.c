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
int start_y;
int start_x;

int camera(WINDOW * win);

extern int mouvementHaut(void);
extern int mouvementGauche(void);
extern int mouvementBas(void);
extern int mouvementDroite(void);
extern int attaqueEpee(void);

extern int texture(WINDOW *win);
extern int creeMap(void);
extern int actualiserMap(void);
extern int textureSimple(WINDOW *win, int i, int j);

int jeu (void) {

    int LINES = getmaxy(stdscr);
    int COLS = getmaxx(stdscr);

    int winHauteur = (int)(LINES * 0.97);
    int winLargeur = (int)(COLS * 0.8);

    // Arrondir winHauteur à un multiple de CASE_HEIGHT
    winHauteur = (winHauteur / CASE_HEIGHT) * CASE_HEIGHT;

    // Arrondir winLargeur à un multiple de CASE_WIDTH
    winLargeur = (winLargeur / CASE_WIDTH) * CASE_WIDTH;

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

    // Texte centré
    char *msg = "Veuillez séléctionner la taille de la fenêtre pour cette partie.";
    char *msg2 = "Elle ne pourra pas être modifiée plus tard et impactera le nombre de choses visibles";
    int taille = strlen(msg);
    int taille2 = strlen(msg2);
	char *msg3 = "Vous vous apprété à lancer le jeu dans une fenètre très petite.";
    int taille3 = strlen(msg3);

    while (1) {
        // Mettre à jour les dimensions de l'écran
        LINES = getmaxy(stdscr);
        COLS = getmaxx(stdscr);

        winHauteur = (int)(LINES * 0.97);
        winLargeur = (int)(COLS * 0.8);

        // Arrondir winHauteur à un multiple de CASE_HEIGHT
        winHauteur = (winHauteur / CASE_HEIGHT) * CASE_HEIGHT;

        // Arrondir winLargeur à un multiple de CASE_WIDTH
        winLargeur = (winLargeur / CASE_WIDTH) * CASE_WIDTH;

        winY = (LINES - winHauteur) / 2;
        winX = (COLS - winLargeur) / 2;

        clear(); // Efface le contenu de la fenêtre principale
        refresh(); // Rafraîchit la fenêtre principale

        // Supprime l'ancienne fenêtre avant d'en créer une nouvelle
        delwin(win);
        
        win = newwin(winHauteur, winLargeur, winY, winX);
        box(win, 0, 0); // Dessine le cadre de la fenêtre
        mvwprintw(win, winHauteur / 2 - 1, (winLargeur / 2) - (taille / 2), "%s", msg);
        mvwprintw(win, winHauteur / 2 + 1, (winLargeur / 2) - (taille2 / 2), "%s", msg2);
        wrefresh(win);

        if (getch() == 10) {// Touche pour sortir = entrée
			if(winHauteur < CASE_HEIGHT * 5 || winLargeur < CASE_WIDTH * 7){ // verif taille écran

				clear(); // Efface le contenu de la fenêtre principale
                refresh(); // Rafraîchit la fenêtre principale

				win = newwin(winHauteur, winLargeur, winY, winX);
                box(win, 0, 0); // Dessine le cadre de la fenêtre

				msg3 = "Vous vous apprété à lancer le jeu dans une fenètre réduite.";
    			taille3 = strlen(msg3);
				mvwprintw(win, winHauteur / 2 - 1, (winLargeur / 2) - (taille3 / 2), "%s", msg3);
				msg3 = "Ce qui risque de le rendre très complex.";
    			taille3 = strlen(msg3);
				mvwprintw(win, winHauteur / 2 , (winLargeur / 2) - (taille3 / 2), "%s", msg3);
				msg3 = "Appuyez sur entré pour confirmé.";
    			taille3 = strlen(msg3);
				mvwprintw(win, winHauteur / 2 + 1, (winLargeur / 2) - (taille3 / 2), "%s", msg3);
				wrefresh(win);
				if (getch() == 10) {// Touche pour sortir = entrée
					break;
           		}
			}
			else{
				break;
			}
            
		}
    }

	start_y = winY;
    start_x = winX;

    //endwin();
	//wclear(win); // Efface le contenu de la fenêtre principale
    //refresh(); // Rafraîchit la fenêtre principale
    //printf("\n\n\n\n\n\n\nnbCaseHauteur = %d \nnbCaseLargeur = %d\n", winHauteur / CASE_HEIGHT, winLargeur / CASE_WIDTH);
	//printf("Hauteur = %d \nLargeur = %d\n\n", winHauteur, winLargeur );

    //return EXIT_SUCCESS;

	if (camera(win) != EXIT_SUCCESS){
		printf("Probleme fonction camera");
		endwin();
		exit(EXIT_FAILURE);
	}

	
	
	// Boucle de jeu ******************************************************************************** /

	//gestion des évènements

	int ch = 0;

	while (ch != 10) {
		ch = getch();
        switch (ch) {
            case 10: /*Pour quitter le jeu*/
				printf("fin jeu\n");
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

			//affichage map
			case 'a':
				WINDOW *winTest = newwin(winHauteur, winLargeur, start_y, start_x);

				box(winTest, 0, 0); // Dessine le cadre de la fenêtre

				// Remplir la fenêtre avec les valeurs définies dans textureSimple
				for (unsigned int i = 1; i < DIMENSION_MAP - 1; i++) {
					for (unsigned int j = 1; j < DIMENSION_MAP - 1; j++) {
						contenuCase = map[i][j];
						if(textureSimple(winTest, i, j) != EXIT_SUCCESS){
							printf("Erreur texture simple");
							exit(EXIT_FAILURE);
						}
					}
				}
				
				// Rafraîchir pour afficher les changements
				wrefresh(winTest);

				// Attendre une entrée de l'utilisateur avant de fermer
				getch();

				// Nettoyer et fermer ncurses
				endwin();
				if (camera(win) != EXIT_SUCCESS){
					printf("Probleme fonction camera");
					endwin();
					exit(EXIT_FAILURE);
				}
				break;

			default:
				if (camera(win) != EXIT_SUCCESS){
					printf("Probleme fonction camera");
					endwin();
					exit(EXIT_FAILURE);
				}
				break;
        }
    }

	endwin();

	return EXIT_SUCCESS;
}

// afficher les images de la camera
int camera(WINDOW *win) {
    int max_y, max_x;
    getmaxyx(win, max_y, max_x);

    int Xcase, Ycase;
    WINDOW *boiteCase;
    tile contenuCase;

    for (Xcase = 0; Xcase < max_x / CASE_WIDTH; Xcase++) {
        for (Ycase = 0; Ycase < max_y / CASE_HEIGHT; Ycase++) {

            contenuCase = map[Xcamera + Xcase][Ycamera + Ycase];
            boiteCase = newwin(CASE_HEIGHT, CASE_WIDTH, start_y + Ycase * CASE_HEIGHT, start_x + Xcase * CASE_WIDTH);
            //box(boiteCase, 0, 0); // Dessine le cadre de la fenêtre
            wrefresh(boiteCase);

            if (texture(boiteCase) != EXIT_SUCCESS) {
                mvwprintw(win, 1, 1, "Fonction texture interrompue");
                delwin(boiteCase);
                return EXIT_FAILURE;
            }
            wrefresh(boiteCase);
            delwin(boiteCase);
        }
    }
    perso.frameAnimation++;
    return EXIT_SUCCESS;
}
