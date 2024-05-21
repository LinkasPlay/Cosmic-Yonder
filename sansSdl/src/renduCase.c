#include <stdlib.h>
#include <unistd.h>

#include <ncurses.h>

#include "texture.h"



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

    int caseLongueur = (WINDOW_WIDTH / 100);
    int caseLargeur = (WINDOW_HEIGHT / 100);
  
	extern int Xcamera;
	extern int Ycamera;
	extern tile **map;
	
	if (perso.lvl == 0){
		if (creeMap() != 0){
			printf("Impossible de cree la map");
		}
	}

	if (camera() != EXIT_SUCCESS){
		printf("Probleme fonction camera");
	}
	
	// Boucle de jeu ******************************************************************************** /

	printf("fini");
	unsigned int frame_limit = 0;

	//gestion des évènements

	int ch;

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
			}
			
        }
    }
	perso.frameAnimation ++;
	return EXIT_SUCCESS;


}

