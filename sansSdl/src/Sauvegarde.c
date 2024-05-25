#include "CosmicYonder.h"  // Inclusion du fichier d'en-tête personnalisé
#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <unistd.h>
#include <string.h>

#include <pthread.h>
#include <pulse/simple.h>
#include <pulse/error.h>

// Déclarations externes des fonctions et variables provenant d'autres fichiers
extern int creeMap(void);
extern int jeu(void);
extern tile contenuCase;
extern int Xcamera;
extern int Ycamera;
extern tile **map;
extern personnage perso;
extern salle room;

// Fonction pour sauvegarder l'état du jeu dans un fichier binaire
void save_game(const char *filename) {
    FILE *file = fopen(filename, "wb");  // Ouverture du fichier en mode écriture binaire
    if (!file) {  // Vérification de l'ouverture du fichier
        perror("Error opening file for writing");  // Affichage d'un message d'erreur en cas d'échec
        return;
    }

    // Écriture des données du personnage dans le fichier
    fwrite(&perso.direction, sizeof(int), 1, file);
    fwrite(&perso.posX, sizeof(int), 1, file);
    fwrite(&perso.posY, sizeof(int), 1, file);
    fwrite(&perso.frameAnimation, sizeof(int), 1, file);
    fwrite(&perso.xp, sizeof(int), 1, file);
    fwrite(&perso.lvl, sizeof(int), 1, file);

    int invSize = 10;  // Taille de l'inventaire
    fwrite(&invSize, sizeof(int), 1, file);  // Écriture de la taille de l'inventaire
    fwrite(perso.inv, sizeof(int), invSize, file);  // Écriture de l'inventaire

    fwrite(&Xcamera, sizeof(int), 1, file);  // Écriture des coordonnées de la caméra
    fwrite(&Ycamera, sizeof(int), 1, file);

    // fwrite(&nbSalles, sizeof(int), 1, file);  // Commenté, peut-être à décommenter si nécessaire

    // Boucle sur les salles du jeu pour écrire leurs données dans le fichier
        // Boucle sur les cases de la salle pour écrire leurs données dans le fichier
        
    for (int j = 0; j < DIMENSION_MAP; ++j) {
        for (int k = 0; k < DIMENSION_MAP; ++k) {
                
            fwrite(&map[j][k].contenu, sizeof(int), 1, file);  // Écriture du contenu de la case
            fwrite(&map[j][k].mstr.hp, sizeof(int), S1, file);  // Écriture de la santé du monstre
            fwrite(&map[j][k].mstr.xp, sizeof(int), 1, file);  // Écriture de l'expérience du monstre
            fwrite(&map[j][k].mstr.loot, sizeof(int), 1, file);  // Écriture du butin du monstre
            fwrite(&map[j][k].mstr.frameAnimation, sizeof(int), 1, file);  // Écriture de l'animation du monstre
            fwrite(&map[j][k].spe.type, sizeof(int), 1, file);  // Écriture du type spécial de la case
            fwrite(&map[j][k].spe.inv, sizeof(int), 1, file);  // Écriture de l'inventaire spécial de la case
        }
    }


    fclose(file);  // Fermeture du fichier
}
