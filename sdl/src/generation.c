#include <SDL.h>
#include <SDL_mixer.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <time.h>

#include "texture.h"

#define LARGEUR_TAB 5
#define LONGUEUR_TAB 5

/*
	Windows : src\*.c -o bin\progMain.exe -I include -L lib -lmingw32 -lSDL2main -lSDL2
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

typedef struct {
    int largeur;
    int longueur;
    int posX;
    int posY;
    int ** cases;
} salle ;

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

salle generation (int graine, salle vaisseau, int numSalle) {
    
    
	return vaisseau;
}

