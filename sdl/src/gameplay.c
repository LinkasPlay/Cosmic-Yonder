#include <SDL.h>
#include <SDL_mixer.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <time.h>

#include "texture.h"

/*
	Windows : gcc src\*.c -o bin\progMain.exe -I include -L lib -lmingw32 -lSDL2main -lSDL2 -mwindows
	Linux : gcc renduCase.c $(sdl2-config __cflags --libs) -o progRenduCase

	Flags render
	SDL_RENDERER_SOFTWARE
	SDL_RENDERER_ACCELERATED
	SDL_RENDERER_PRESENTVSYNC
	SDL_RENDERER_TARGETTEXTURE
*/

extern void SDL_ExitWithError(const char *message);
extern int texture( int argc, char **argv);
extern int creeMap(void);
extern int actualiserMap(void);

extern int Xcamera;
extern int Ycamera;

personnage perso;
personnage persoPast;

int mouvementHaut(void){
    if ( (Ycamera != 0) ){
        Ycamera = Ycamera - 1;
    }
    if (perso.posY > 1){
        persoPast.posX = perso.posX;
        persoPast.posY = perso.posY;
        perso.direction = 1;
        perso.posY = perso.posY - 1;
    }
    if (actualiserMap() != EXIT_SUCCESS){
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

int mouvementGauche(void){
    if ( (Xcamera != 0) ) {
        Xcamera = Xcamera - 1;
    }
    if (perso.posX > 1){
        persoPast.posX = perso.posX;
        persoPast.posY = perso.posY;
        perso.direction = 2;
        perso.posX = perso.posX - 1;
    }
    if (actualiserMap() != EXIT_SUCCESS){
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

int mouvementBas(void){
    if ( (Ycamera < (DIMENSION_MAP - (WINDOW_HEIGHT / 100) ) ) ){
        Ycamera = Ycamera + 1;
    }
    if (perso.posY < DIMENSION_MAP - 2){
        persoPast.posX = perso.posX;
        persoPast.posY = perso.posY;
        perso.direction = 3;
        perso.posY = perso.posY + 1;
    }
    if (actualiserMap() != EXIT_SUCCESS){
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

int mouvementDroite(void){
    if ( (Xcamera < (DIMENSION_MAP - (WINDOW_WIDTH / 100) ) ) ) {
        Xcamera = Xcamera + 1;
    }
    if (perso.posX < DIMENSION_MAP - 2){
        persoPast.posX = perso.posX;
        persoPast.posY = perso.posY;
        perso.direction = 4;
        perso.posX = perso.posX + 1;
    }
    if (actualiserMap() != EXIT_SUCCESS){
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}


