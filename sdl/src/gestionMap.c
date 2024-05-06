#include <SDL.h>
#include <SDL_mixer.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <time.h>

#include "texture.h"


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

extern struct salle;
extern int room;

int Xcamera = DIMENSION_MAP / 2;
int Ycamera = DIMENSION_MAP / 2;
int **map;

int creeMap(void) {

    int **p = malloc(sizeof(int*[DIMENSION_MAP]));

    if (p == NULL) {
        printf("Échec de l'allocation\n");
        return EXIT_FAILURE;
    }

    for (unsigned i = 0; i < DIMENSION_MAP; ++i) {
        p[i] = malloc(sizeof(int[DIMENSION_MAP]));

        if (p[i] == NULL) {
            printf("Échec de l'allocation\n");
            return EXIT_FAILURE;
        }
    }

    for (unsigned i = 0; i < DIMENSION_MAP; ++i) {
        for (unsigned j = 0; j < DIMENSION_MAP; ++j) {
            p[i][j] = -5;
            //printf("[%d] ",p[i][j]);
        }
    }

    map = p;

    for (unsigned i = 0; i < 3; ++i) {
        free(p[i]);
    }

    free(p);
    return 0;
}



