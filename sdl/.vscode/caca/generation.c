#include <SDL.h>
#include <SDL_mixer.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <time.h>

#include "texture.h"

#define LARGEUR_TAB 5
#define LONGUEUR_TAB 5

#define LIGNES 5
#define COLONNES 5

/*
	Windows : gcc src\*.c -o bin\progMain.exe -I include -L lib -lmingw32 -lSDL2main -lSDL2 -mwindows
	Windows sans terminal qui ouvre : gcc src/generation.c -o bin\progGeneration -mwindows
	Linux : gcc generation.c -o progGeneration

	Flags render
	SDL_RENDERER_SOFTWARE
	SDL_RENDERER_ACCELERATED
	SDL_RENDERER_PRESENTVSYNC
	SDL_RENDERER_TARGETTEXTURE
*/

//message erreur
void erreur(char message){
	printf("ERREUR : %s\n",message);
	exit(0);
}

int creeTab(void) {

    int largeurTab, longueurTab;
    printf("Quelle est la largeur du carré ?");
    scanf("%d",(&largeurTab) - 1);
    printf("Quelle est la longueur du carré ?");
    scanf("%d",(&longueurTab) - 1);

    int **p = malloc(sizeof(int*[largeurTab]));

    if (p == NULL) {
        printf("Échec de l'allocation\n");
        return EXIT_FAILURE;
    }

    for (unsigned i = 0; i < longueurTab; ++i) {
        p[i] = malloc(sizeof(int[longueurTab]));

        if (p[i] == NULL) {
            printf("Échec de l'allocation\n");
            return EXIT_FAILURE;
        }
    }

    for (unsigned i = 0; i < largeurTab; ++i) {
        for (unsigned j = 0; j < longueurTab; ++j) {
            p[i][j] = (i * longueurTab) + j;
            printf("p[%u][%u] = %d\n", i, j, p[i][j]);
        }
    }

    for (unsigned i = 0; i < 3; ++i) {
        free(p[i]);
    }

    free(p);
    return 0;
}

/*
J'ai fais un programme qui va creer un tableau de 5 par 5 ou chaque element va etre remplacer par une image.
Aussi la case au centre tout en haut est une porte pour acceder au niveau suivant.
*/
salle generation () {
    
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("Erreur lors de l'initialisation de SDL : %s\n", SDL_GetError());
        return 1;
    }

    SDL_Surface* image = SDL_LoadBMP("votre_image.bmp");
    if (image == NULL) {
        printf("Erreur lors du chargement de l'image : %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Surface* tableau[LIGNES][COLONNES];

    for (int i = 0; i < LIGNES; i++) {
        for (int j = 0; j < COLONNES; j++) {
            if (i == 2 && j == 0) {
                tableau[i][j] = image;
                SDL_IncrementRefcount(image);
            } else {
                tableau[i][j] = SDL_LoadBMP("autre_image.bmp");
                if (tableau[i][j] == NULL) {
                    printf("Erreur lors du chargement de l'image : %s\n", SDL_GetError());
                    SDL_Quit();
                    return 1;
                }
            }
        }
    }

    SDL_FreeSurface(image);

    printf("Tableau de 5 par 5 :\n");
    for (int i = 0; i < LIGNES; i++) {
        for (int j = 0; j < COLONNES; j++) {
            if (tableau[i][j] == image) {
                printf("porte");
            } else {
                printf("autre_image");
            }
        }
        printf("\n");
    }

    for (int i = 0; i < LIGNES; i++) {
        for (int j = 0; j < COLONNES; j++) {
            if (tableau[i][j] != image) {
                SDL_FreeSurface(tableau[i][j]);
            }
        }
    }

    SDL_Quit();

    return 0;
}
    
	return vaisseau;

