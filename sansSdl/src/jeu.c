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

tile contenuCase;
extern void* play_music(void* arg);
extern bool stop_music;
//void afficher_barre_experience(WINDOW *win, int start_y, int start_x);

extern personnage perso;
int start_y;
int start_x;

extern int Xcamera;
extern int Ycamera;

int camera(WINDOW * win);

extern int mouvementHaut(void);
extern int mouvementGauche(void);
extern int mouvementBas(void);
extern int mouvementDroite(void);
extern int attaqueEpee(void);
extern int monstreMouvement(int x, int y);

extern int texture(WINDOW *win);
extern int creeMap(void);
extern int actualiserMap(void);
extern int textureSimple(WINDOW *win, int i, int j);
extern void afficherMap(void);
extern void debug(char * msg);

void afficher_coeurs(WINDOW *win, const char *filename, int start_y, int start_x);
void afficher_inventaire(WINDOW * win);
void affiche_barre_experience(WINDOW *win, int niveau, int experience, int experience_necessaire);

void* timer_thread(void * arg);
void start_timer(timer_t* timer_data);
void afficher_timer(WINDOW *win, int minutes);

int jeu (void){

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
				msg3 = "Ce qui risque de le rendre très instable.";
    			taille3 = strlen(msg3);
				mvwprintw(win, winHauteur / 2 , (winLargeur / 2) - (taille3 / 2), "%s", msg3);
				msg3 = "Appuyez sur entrer pour confirmé.";
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

    if (perso.lvl == 0) {
        if (creeMap() != 0) {
            printf("Impossible de créer la map\n");
            endwin();
            return 1;
        }
		debug("Map créé");
    }

	char str[20];
	sprintf(str, "%d, %d, %d", map[perso.posX][perso.posY].contenu, perso.posX, perso.posY);
	debug(str);
	getch();

	Xcamera = perso.posX - 4;
	Ycamera = perso.posY - 3;

	// Rafraîchir pour afficher les changements
	wrefresh(win);
	

	if (camera(win) != EXIT_SUCCESS){
		printf("Probleme fonction camera");
		endwin();
		exit(EXIT_FAILURE);
	}

	wrefresh(win);
	
	// Boucle de jeu ******************************************************************************** /

	//gestion des évènements

	int ch = 0;

	while (ch != 10) {
		ch = getch();
        switch (ch) {
			case 9:
				switch(pause()){
					case -1:
						ch = 10;
						break;
					case EXIT_FAILURE:
						printf("Erreur pause.");
						break;
				}
				ch = 9;
				win = newwin(winHauteur, winLargeur, winY, winX);
        		box(win, 0, 0); // Dessine le cadre de la fenêtre
				wrefresh(win);
				continue;
				
            case 10: /*Pour quitter le jeu*/
				printf("fin jeu\n");
				endwin();
				break;

			//afficher la carte
			case 'm':
				afficherMap();
				continue;

			//attaque
			case 'e':
            case ' ':
				if(attaqueEpee() != EXIT_SUCCESS){
					printf("Erreur attaque");
				}
				if (camera(win) != EXIT_SUCCESS){
					printf("Probleme fonction camera");
					endwin();
					exit(EXIT_FAILURE);
				}
				continue;

			//mvouvement haut
			case 'z':
			case KEY_UP:
				if(mouvementHaut() != EXIT_SUCCESS){
					printf("Erreur mouvement haut");
				}
				if (camera(win) != EXIT_SUCCESS){
					printf("Probleme fonction camera");
					endwin();
					exit(EXIT_FAILURE);
				}
				continue;
            
			//mvouvement gauche
			case 'q':
			case KEY_LEFT:
				if(mouvementGauche() != EXIT_SUCCESS){
					printf("Erreur mouvement gauche");
				}
				if (camera(win) != EXIT_SUCCESS){
					printf("Probleme fonction camera");
					endwin();
					exit(EXIT_FAILURE);
				}
				continue;

			//mvouvement bas
			case 's':
			case KEY_DOWN:
				if(mouvementBas() != EXIT_SUCCESS){
					printf("Erreur mouvement bas");
				}
				if (camera(win) != EXIT_SUCCESS){
					printf("Probleme fonction camera");
					endwin();
					exit(EXIT_FAILURE);
				}
				continue;

			//mvouvement droite
			case 'd':
			case KEY_RIGHT:
				if(mouvementDroite() != EXIT_SUCCESS){
					printf("Erreur mouvement droite");
				}
				if (camera(win) != EXIT_SUCCESS){
					printf("Probleme fonction camera");
					endwin();
					exit(EXIT_FAILURE);
				}
				continue;

			//affichage map
			case 'a':
				WINDOW *winTest = newwin(winHauteur, winLargeur, start_y, start_x);

				box(winTest, 0, 0); // Dessine le cadre de la fenêtre

				// Remplir la fenêtre avec les valeurs définies dans textureSimple
				for (unsigned int i = perso.posY - 15; i <= perso.posY + 15; i++) {
					for (unsigned int j = perso.posX - 25; j <= perso.posX + 25; j++) {
						// Vérifiez que les indices ne dépassent pas les limites du tableau map
						if (i >= 0 && i < DIMENSION_MAP && j >= 0 && j < DIMENSION_MAP) {
							contenuCase = map[i][j];
							if (textureSimple(winTest, i, j) != EXIT_SUCCESS) {
								printf("Erreur texture simple");
								exit(EXIT_FAILURE);
							}
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
int camera(WINDOW *win){
	debug("Camerara");
    int max_y, max_x;
	char str[20];
    getmaxyx(win, max_y, max_x);

    int Xcase, Ycase;
    WINDOW *boiteCase;

	//contenuCase.contenu = 1;

    for (Xcase = 0; Xcase < max_x / CASE_WIDTH; Xcase++) {
        for (Ycase = 0; Ycase < max_y / CASE_HEIGHT; Ycase++) {

			contenuCase.contenu = map[Xcamera + Xcase][Ycamera + Ycase].contenu;
			
            boiteCase = newwin(CASE_HEIGHT, CASE_WIDTH, start_y + Ycase * CASE_HEIGHT, start_x + Xcase * CASE_WIDTH);
            //box(boiteCase, 0, 0); // Dessine le cadre de la fenêtre
            //wrefresh(boiteCase);
			if(contenuCase.contenu == 2){
				if (monstreMouvement(Xcamera + Xcase, Ycamera + Ycase) != EXIT_SUCCESS){
					printf("Erreur mouvement monstre");
					return EXIT_FAILURE;
				}
			}
			sprintf(str, "%d, %d, %d", contenuCase.contenu, Xcamera + Xcase, Ycamera + Ycase);
			debug(str);
			//getch();

            if (texture(boiteCase) != EXIT_SUCCESS) {
                mvwprintw(win, 1, 1, "Fonction texture interrompue");
                delwin(boiteCase);
                return EXIT_FAILURE;
            }
            
            delwin(boiteCase);
        }
    }
	wrefresh(boiteCase);
    perso.frameAnimation++;

	WINDOW * timer = newwin(3,20,4,185);

	afficher_timer(timer,7);
	delwin(timer);
	WINDOW *status_win = newwin(3, 20, 4, 0); //Fenêtre pour les coeurs
    afficher_coeurs(status_win, "image/coeur.txt", 0, 0);
    delwin(status_win);
	WINDOW *inventory_win = newwin(13, 23, 18, 0); // Fenêtre pour l'inventaire
    afficher_inventaire(inventory_win);
    delwin(inventory_win);
	WINDOW *exp_win = newwin(3,20,8,185);
	affiche_barre_experience(exp_win,1,0,50);
	delwin(exp_win);
    return EXIT_SUCCESS;
}


//test