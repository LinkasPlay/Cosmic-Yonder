#include <stdio.h>
#include <stdlib.h>

// Déclarations incomplètes des structures
typedef struct personnage personnage;
typedef struct salle salle;
typedef struct monstre monstre;
typedef struct special special;
typedef struct tile tile;

// Fonction pour sauvegarder les données du jeu
void save_game(const char *filename, personnage *perso, salle *room, int nbSalles) {
    FILE *file = fopen(filename, "wb");
    if (!file) {
        perror("Error opening file for writing");
        return;
    }

    // Sauvegarde des données du personnage
    fwrite(perso, sizeof(personnage), 1, file);

    // Sauvegarde du nombre de salles
    fwrite(&nbSalles, sizeof(int), 1, file);

    // Sauvegarde des données de chaque salle
    for (int i = 0; i < nbSalles; ++i) {
        salle *s = &room[i];
        fwrite(s, sizeof(salle), 1, file);

        // Sauvegarde des données de chaque case de la salle
        for (int j = 0; j < s->largeur; ++j) {
            fwrite(s->cases[j], sizeof(tile), s->longueur, file);
        }
    }

    fclose(file);
}

int main() {
    // Initialisation des données du personnage
    personnage perso = {1, 10, 10, 1, 0, 1, NULL};

    // Initialisation des données des salles
    int nbSalles = 2;
    salle room[2];
    for (int i = 0; i < nbSalles; ++i) {
        room[i].num = i + 1;
        room[i].largeur = 5;
        room[i].longueur = 5;
        room[i].posX = 0;
        room[i].posY = 0;
        room[i].cases = (tile **)malloc(room[i].largeur * sizeof(tile *));
        for (int j = 0; j < room[i].largeur; ++j) {
            room[i].cases[j] = (tile *)malloc(room[i].longueur * sizeof(tile));
            for (int k = 0; k < room[i].longueur; ++k) {
                room[i].cases[j][k].contenu = 0;
            }
        }
    }

    // Appel de la fonction pour sauvegarder les données du jeu
    save_game("Fichierdesauvegarde.txt", &perso, room, nbSalles);

    // Libération de la mémoire allouée pour les salles
    for (int i = 0; i < nbSalles; ++i) {
        for (int j = 0; j < room[i].largeur; ++j) {
            free(room[i].cases[j]);
        }
        free(room[i].cases);
    }

    return 0;
}
