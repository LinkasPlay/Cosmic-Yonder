#include "cosmicYonder.h"
#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <unistd.h>
#include <string.h>

#include <pthread.h>
#include <pulse/simple.h>
#include <pulse/error.h>

extern int creeMap(void);
extern int jeu(void);
extern tile contenuCase;
extern int Xcamera;
extern int Ycamera;
extern tile **map;
extern personnage perso;
extern salle room;

void save_game(const char *filename) {
    FILE *file = fopen(filename, "wb");
    if (!file) {
        perror("Error opening file for writing");
        return;
    }

    fwrite(&perso.direction, sizeof(int), 1, file);
    fwrite(&perso.posX, sizeof(int), 1, file);
    fwrite(&perso.posY, sizeof(int), 1, file);
    fwrite(&perso.frameAnimation, sizeof(int), 1, file);
    fwrite(&perso.xp, sizeof(int), 1, file);
    fwrite(&perso.lvl, sizeof(int), 1, file);

    int invSize = 10;
    fwrite(&invSize, sizeof(int), 1, file);
    fwrite(perso.inv, sizeof(int), invSize, file);

    fwrite(&Xcamera, sizeof(int), 1, file);
    fwrite(&Ycamera, sizeof(int), 1, file);

    //fwrite(&nbSalles, sizeof(int), 1, file);

    /*

    for (int i = 0; i < nbSalles; ++i) {
        salle *s = room.cases[i];
        fwrite(&s->num, sizeof(int), 1, file);
        fwrite(&s->largeur, sizeof(int), 1, file);
        fwrite(&s->longueur, sizeof(int), 1, file);
        fwrite(&s->posX, sizeof(int), 1, file);
        fwrite(&s->posY, sizeof(int), 1, file);

        for (int j = 0; j < s->largeur; ++j) {
            for (int k = 0; k < s->longueur; ++k) {
                tile *t = &s->cases[j][k];
                fwrite(&t->contenu, sizeof(int), 1, file);
                fwrite(&t->mstr.hp, sizeof(int), 1, file);
                fwrite(&t->mstr.xp, sizeof(int), 1, file);
                fwrite(&t->mstr.loot, sizeof(int), 1, file);
                fwrite(&t->mstr.frameAnimation, sizeof(int), 1, file);
                fwrite(&t->spe.type, sizeof(int), 1, file);
                fwrite(&t->spe.inv, sizeof(int), 1, file);
            }
        }
    }

    */

    fclose(file);
}
