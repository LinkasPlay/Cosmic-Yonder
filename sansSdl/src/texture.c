#include "CosmicYonder.h"
#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <unistd.h>
#include <string.h>

#include <pthread.h>
#include <pulse/simple.h>
#include <pulse/error.h>
#define FRAME_IN_ANIMATION 2

// COMMANDE TERMINAL : gcc -o ProgMain *.c -lncurses -lm -lpthread -lpulse-simple -lpulse

//Taille des cases = 5 x 10

extern personnage perso;

int afficher_image_ascii(WINDOW *win, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        return EXIT_FAILURE;
    }

    int ch;
    int row = 0, col = 0;
    werase(win);

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
    wrefresh(win);
    return EXIT_SUCCESS;
}

int texture(WINDOW *win) {
    extern tile contenuCase;
    char *image;

    switch (contenuCase.contenu) {
        case -5:
            image = "image/espace.txt";
            break;
        case -2:
            image = "image/mur.txt";
            break;
        case -1:
            image = "image/porte.txt";
            break;
        case 0:
            image = "image/sol.txt";
            break;
        case 1:
            switch (perso.direction) {
                case 1:
                    if (perso.frameAnimation < FRAME_IN_ANIMATION + 1)
                        image = "image/personnage/personnageHaut (1).txt";
                    else if (perso.frameAnimation < FRAME_IN_ANIMATION * 2 + 1)
                        image = "image/personnage/personnageHaut (2).txt";
                    else if (perso.frameAnimation < FRAME_IN_ANIMATION * 3 + 1)
                        image = "image/personnage/personnageHaut (3).txt";
                    else
                        image = "image/personnage/personnageHaut (4).txt";
                    break;
                case 2:
                    if (perso.frameAnimation < FRAME_IN_ANIMATION + 1)
                        image = "image/personnage/personnageGauche (1).txt";
                    else if (perso.frameAnimation < FRAME_IN_ANIMATION * 2 + 1)
                        image = "image/personnage/personnageGauche (2).txt";
                    else if (perso.frameAnimation < FRAME_IN_ANIMATION * 3 + 1)
                        image = "image/personnage/personnageGauche (3).txt";
                    else
                        image = "image/personnage/personnageGauche (4).txt";
                    break;
                case 3:
                    if (perso.frameAnimation < FRAME_IN_ANIMATION + 1)
                        image = "image/personnage/personnageBas (1).txt";
                    else if (perso.frameAnimation < FRAME_IN_ANIMATION * 2 + 1)
                        image = "image/personnage/personnageBas (2).txt";
                    else if (perso.frameAnimation < FRAME_IN_ANIMATION * 3 + 1)
                        image = "image/personnage/personnageBas (3).txt";
                    else
                        image = "image/personnage/personnageBas (4).txt";
                    break;
                case 4:
                    if (perso.frameAnimation < FRAME_IN_ANIMATION + 1)
                        image = "image/personnage/personnageDroite (1).txt";
                    else if (perso.frameAnimation < FRAME_IN_ANIMATION * 2 + 1)
                        image = "image/personnage/personnageDroite (2).txt";
                    else if (perso.frameAnimation < FRAME_IN_ANIMATION * 3 + 1)
                        image = "image/personnage/personnageDroite (3).txt";
                    else
                        image = "image/personnage/personnageDroite (4).txt";
                    break;
                default:
                    image = "image/espace.txt"; // Case défaut au cas ou il y a un pb
                    break;
            }
            if (perso.frameAnimation >= FRAME_IN_ANIMATION * 4) {
                perso.frameAnimation = 0;
            }
            break;
        case 2:
            image = "image/monstre.txt";
            break;
        case 3:
            image = "image/machine.txt";
            break;
        default:
            image = "image/espace.txt"; // Case défaut au cas ou il y a un pb
            break;
    }

    if (afficher_image_ascii(win, image) != EXIT_SUCCESS) {
        return EXIT_FAILURE;
    }

    wrefresh(win);
    return EXIT_SUCCESS;
}

//texture de l'ui

int textureSimple(WINDOW *win, int i, int j) {
    extern tile contenuCase;
    char *image;

    switch (contenuCase.contenu) {
        case -5:
            image = ".";
            break;
        case -2:
            image = "#";
            break;
        case -1:
            image = "%";
            break;
        case 0:
            image = " ";
            break;
        case 1:
            switch (perso.direction) {
                case 1:
                    image = "1";
                    break;
                case 2:
                    image = "2";
                    break;
                case 3:
                    image = "3";
                    break;
                case 4:
                    image = "4";
                    break;
                default:
                    image = "?"; // Case défaut au cas ou il y a un pb
                    break;
            }
            break;
        case 2:
            image = "x";
            break;
        case 3:
            image = "o";
            break;
        default:
        	image = "?"; // Case défaut au cas ou il y a un pb
        	break;
    }

	mvwprintw(win, i, j, "%s", image);

    wrefresh(win);
    return EXIT_SUCCESS;
}

