#include <stdlib.h>
#include <unistd.h>

#include <ncurses.h>

#include "texture.h"

#define FRAME_IN_ANIMATION 2

// COMMANDE TERMINAL : gcc -o ProgMain *.c -lncurses -lm -lpthread -lpulse-simple -lpulse

extern personnage perso;

void afficher_image_ascii(WINDOW *win, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        return;
    }

    int ch;
    int row = 0, col = 0;

    // Efface la fenêtre avant d'afficher l'image
    werase(win);

    // Lire le fichier caractère par caractère et afficher dans la fenêtre
    while ((ch = fgetc(file)) != EOF) {
        if (ch == '\n') {
            row++;
            col = 0;
        } else {
            mvwaddch(win, row, col, ch);
            col++;
        }
    }

    fclose(file);

    // Rafraîchir la fenêtre pour afficher l'image
    wrefresh(win);
}

int texture(WINDOW *win) {
	
    //Affichage
	extern tile contenuCase;
	char * image;

    //Chargement image selon son contenu
    switch (contenuCase.contenu) {
		// case avec espace
        case -5:
            afficher_image_ascii(win, "image/espace.txt");
			break;
        // case avec mur
        case -2:
            afficher_image_ascii(win, "image/mur.txt");
        	break;
        // case avec porte
        case -1:
            afficher_image_ascii(win, "image/porte.txt");
			break;
        // case vide
        case 0:
            afficher_image_ascii(win, "image/sol.txt");
			break;
        // case avec personnage
        case 1:
			// choix image selon direction et frame
			switch (perso.direction){
			
			// direction = haut
			case 1:
			
				// frame 1 - 10 => image 1
				if (perso.frameAnimation < FRAME_IN_ANIMATION + 1 ){
					image = "image/personnage/personnageHaut (1).txt";
				}

				// frame 11 - 20 => image 2
				else if (perso.frameAnimation < FRAME_IN_ANIMATION * 2 + 1 ){
					image = "src/image/personnage/personnageHaut (2).txt";
				}

				// frame 21 - 30 => image 3
				else if (perso.frameAnimation < FRAME_IN_ANIMATION * 3 + 1 ){
					image = "image/personnage/personnageHaut (3).txt";
				}
				// frame 31 - 40 => image 4
				else {
					image = "image/personnage/personnageHaut (4).txt";
				}
				break;

			// direction = gauche
			case 2:
				
				// frame 1 - 10 => image 1
				if (perso.frameAnimation < FRAME_IN_ANIMATION + 1 ){
					image = "src/image/personnage/personnageGauche (1).txt";
				}

				// frame 11 - 20 => image 2
				else if (perso.frameAnimation < FRAME_IN_ANIMATION * 2 + 1 ){
					image = "src/image/personnage/personnageGauche (2).txt";
				}

				// frame 21 - 30 => image 3
				else if (perso.frameAnimation < FRAME_IN_ANIMATION * 3 + 1 ){
					image = "src/image/personnage/personnageGauche (3).txt";
				}
				// frame 31 - 40 => image 4
				else {
					image = "src/image/personnage/personnageGauche (4).txt";
				}
				break;

			// direction = bas
			case 3:
				
				// frame 1 - 10 => image 1
				if (perso.frameAnimation < FRAME_IN_ANIMATION + 1 ){
					image = "image/personnage/personnageBas (1).txt";
				}

				// frame 11 - 20 => image 2
				else if (perso.frameAnimation < FRAME_IN_ANIMATION * 2 + 1 ){
					image = "image/personnage/personnageBas (2).txt";
				}

				// frame 21 - 30 => image 3
				else if (perso.frameAnimation < FRAME_IN_ANIMATION * 3 + 1 ){
					image = "image/personnage/personnageBas (3).txt";
				}
				// frame 31 - 40 => image 4
				else {
					image = "image/personnage/personnageBas (4).txt";
				}
				break;

			// direction = droite
			case 4:
				
				// frame 1 - 10 => image 1
				if (perso.frameAnimation < FRAME_IN_ANIMATION + 1 ){
					image = "image/personnage/personnageDroite (1).txt";
				}

				// frame 11 - 20 => image 2
				else if (perso.frameAnimation < FRAME_IN_ANIMATION * 2 + 1 ){
					image = "image/personnage/personnageDroite (2).txt";
				}

				// frame 21 - 30 => image 3
				else if (perso.frameAnimation < FRAME_IN_ANIMATION * 3 + 1 ){
					image = "image/personnage/personnageDroite (3).txt";
				}
				// frame 31 - 40 => image 4
				else {
					image = "image/personnage/personnageDroite (4).txt";
				}
				break;
			
			default:
				break;
			}
            afficher_image_ascii(win, image);
			if (perso.frameAnimation >= FRAME_IN_ANIMATION * 4){
				perso.frameAnimation = 0;
			}
			break;
        // case avec monstre
        case 2:
            afficher_image_ascii(win, "image/monstre.txt");
			break;
        // case avec machine
        case 3:
            afficher_image_ascii(win, "image/machine.txt");
        	break;
		default:
			break;
    }

    wrefresh(win);
    
	return EXIT_SUCCESS;
}

//texture de l'ui



