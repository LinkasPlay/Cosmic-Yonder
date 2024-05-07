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

/*
Expliquation gameplay du jeu :

    on commence avec l'invetaire vide
    1) l'épée tuyau perme de fraper autour de soi, fait 50 de dégats
    2 ) le pistolet tire sur une ligne droite devant soi, fait 20 dégats
    3) les potions de vies redonnes 2 coeurs au joueur
    4) les potions d'xp donne 1/4 de l'xp nécessaire pour ganer un niveau
    5) les clé a molette servent à réparé les machines
    6) les clés servent à ouvrir les coffres
    7) la grande clé sert à ouvrir la dernière salle

    une épée/tuyau est généré dans l'une des 5 premières salles (1/3 chance par sale, 1/1 si salle 5)
    une salle de boss, fermé avec une porte spéciale est générée dans les alles 10 à 25 ==> vérifié qu'il y a des portes libres autrepart pour ne pas bloquer joueur

    on trouve forcement un pistole dans l'un des 10 premiers coffres ouverts
    on trouve la clé du boss dans l'un des coffres entre le 10eme et 20eme
    les coffres on 40% de chance de donner une clé a molette, 30% pour une potion de vie, 30% pour une potion d'xp

    utiliser une clé à molette sur une machine permet de gagner 3minutes en plus au chrono total
    il faut utiliser 2 clé a molette sur les grandes machines pour les réparés
    il faut réparé 5 grandes machines pour avoir la vrai fin

    il y a quatre type de monstres :
        niv 1 : 80 pv, 10 xp
        niv 2 : 180 pv, 20 xp | drop : clé * 0.05
        niv 3 : 260 pv, 30 xp | drop : clé * 0.1, clé a molette * 0.1
        niv 5 (boss) : 1000 pv, 0 xp => fin du jeu

*/

extern void SDL_ExitWithError(const char *message);
extern int texture( int argc, char **argv);
extern int creeMap(void);
extern int actualiserMap(void);

extern int Xcamera;
extern int Ycamera;

personnage perso;
personnage persoPast;

int attaqueEpee(void){

    switch (perso.direction) {
        
        //vers le haut
        case 1 :
            


    }



}

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


