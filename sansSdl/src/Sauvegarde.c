#include <stdio.h>
#include <stdlib.h>

typedef struct personnage personnage;
typedef struct monstre monstre;
typedef struct special special;
typedef struct tile tile;
typedef struct salle salle;

extern int creeMap(void);
extern int jeu(int argc, char **argv);
extern tile contenuCase;
extern int Xcamera;
extern int Ycamera;
extern tile **map;
extern personnage hero;
extern salle *salles;
extern int nbSalles;

void save_game(const char *filename) {
    FILE *file = fopen(filename, "wb");
    if (!file) {
        perror("Error opening file for writing");
        return;
    }

    fwrite(&hero.direction, sizeof(int), 1, file);
    fwrite(&hero.posX, sizeof(int), 1, file);
    fwrite(&hero.posY, sizeof(int), 1, file);
    fwrite(&hero.frameAnimation, sizeof(int), 1, file);
    fwrite(&hero.xp, sizeof(int), 1, file);
    fwrite(&hero.lvl, sizeof(int), 1, file);

    int invSize = 10;
    fwrite(&invSize, sizeof(int), 1, file);
    fwrite(hero.inv, sizeof(int), invSize, file);

    fwrite(&Xcamera, sizeof(int), 1, file);
    fwrite(&Ycamera, sizeof(int), 1, file);

    fwrite(&nbSalles, sizeof(int), 1, file);

    for (int i = 0; i < nbSalles; ++i) {
        salle *s = &salles[i];
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

    fclose(file);
}
