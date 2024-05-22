#include <stdlib.h>
#include <unistd.h>

#include <ncurses.h>

#include "texture.h"


// COMMANDE TERMINAL : gcc -o ProgMain *.c -lncurses -lm -lpthread -lpulse-simple -lpulse
//message erreur

extern int generation(int longueur, int largeur, int num_salle, int cote);

personnage perso;
personnage persoPast;
extern int room;
int inv[7];

int Xcamera = (DIMENSION_MAP / 2) - 7;
int Ycamera = (DIMENSION_MAP / 2) - 4;
tile **map;

int creeMap(void) {

    // initialisation perso
    perso.posX = (DIMENSION_MAP / 2);
    perso.posY = (DIMENSION_MAP / 2);
    perso.direction = 3;
    perso.frameAnimation = 0;
    perso.inv = inv;
    perso.lvl = 1;
    perso.xp = 0;

    // Allocation de mémoire pour map
    map = malloc(sizeof(tile*[DIMENSION_MAP]));

    if (map == NULL) {
        printf("Échec de l'allocation\n");
        return EXIT_FAILURE;
    }

    for (unsigned i = 0; i < DIMENSION_MAP; ++i) {
        map[i] = malloc(sizeof(tile[DIMENSION_MAP]));

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

    if (generation(5, 5, 1, 0) != EXIT_SUCCESS){
        printf("Erreur generation salle 1");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int liberationMap(void){
    for (unsigned i = 0; i < DIMENSION_MAP; ++i) {
        free(map[i]);
    }

    free(map);
    return EXIT_SUCCESS;
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


