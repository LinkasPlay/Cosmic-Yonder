#include <SDL.h>
#include <SDL_mixer.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <time.h>

#include "texture.h"


/*
	Windows : gcc src\*.c -o bin\progMain.exe -I include -L lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_mixer -mwindows
	Windows sans terminal qui ouvre : gcc src/generation.c -o bin\progGeneration -mwindows
	Linux : gcc generation.c -o progGeneration

	Flags render
	SDL_RENDERER_SOFTWARE
	SDL_RENDERER_ACCELERATED
	SDL_RENDERER_PRESENTVSYNC
	SDL_RENDERER_TARGETTEXTURE
*/

//message erreur

/*/
typedef struct personnage {
    int direction; //haut = 1, gauche = 2, bas = 3, droite = 4
    int posX;
    int posY;
    int frameAnimation;
} personnage;

typedef struct salle {
    int largeur;
    int longueur;
    int posX;
    int posY;
    int ** cases;
} salle ;
*/

extern personnage perso;
extern personnage persoPast;
extern int room;

int Xcamera = (DIMENSION_MAP / 2) - 7;
int Ycamera = (DIMENSION_MAP / 2) - 4;
tile **map;

int creeMap(void) {
    perso.posX = (DIMENSION_MAP / 2);
    perso.posY = (DIMENSION_MAP / 2);
    perso.direction = 3;
    perso.frameAnimation = 0;

    // Allocation de mémoire pour map
    map = malloc(sizeof(int*[DIMENSION_MAP]));

    if (map == NULL) {
        printf("Échec de l'allocation\n");
        return EXIT_FAILURE;
    }

    for (unsigned i = 0; i < DIMENSION_MAP; ++i) {
        map[i] = malloc(sizeof(int[DIMENSION_MAP]));

        if (map[i] == NULL) {
            printf("Échec de l'allocation\n");
            return EXIT_FAILURE;
        }
    }

    for (unsigned i = 0; i < DIMENSION_MAP; ++i) {
        for (unsigned j = 0; j < DIMENSION_MAP; ++j) {
            map[i][j].contenu = -5;
            if ( (i == perso.posX) && (j == perso.posY) ){
                map[i][j].contenu = 1;
            }
        }
    }

    return EXIT_SUCCESS;
}

int liberationMap(void){
    for (unsigned i = 0; i < DIMENSION_MAP; ++i) {
        free(map[i]);
    }

    free(map);
    return 0;
}

int actualiserMap(void){
    for (unsigned i = 0; i < DIMENSION_MAP; ++i) { // pos X case
        for (unsigned j = 0; j < DIMENSION_MAP; ++j) { // pos Y case
            if ( (i == perso.posX) && (j == perso.posY) ){
                map[i][j].contenu = 1;
            }
            else if ( (i == persoPast.posX) && (j == persoPast.posY) ){
                map[i][j].contenu = 0;
            }
            
            //printf("[%d] ",p[i][j]);
        }
    }

    return EXIT_SUCCESS;

}



