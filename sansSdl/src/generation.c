#include <stdlib.h>
#include <unistd.h>

#include <ncurses.h>

#include "CosmicYonder.h"
#include <math.h>

#define OBJ_MAX 9

#define LARGEUR_TAB 5
#define LONGUEUR_TAB 5

#define LIGNES 5
#define COLONNES 5

extern int mouvementHaut(void);
extern int mouvementGauche(void);
extern int mouvementBas(void);
extern int mouvementDroite(void);
extern void debug(char * msg);

int porteLibre = 0;
extern personnage perso;
extern personnage persoPast;

// COMMANDE TERMINAL : gcc -o ProgMain *.c -lncurses -lm -lpthread -lpulse-simple -lpulse

extern int graine;
int entreeX;
int entreeY;

extern salle room;

void ajouterSalle (void);

void freeRoomCases(tile **cases, int largeur) {
    if (cases) {
        for (int i = 0; i < largeur; i++) {
            free(cases[i]);
        }
        free(cases);
    }
}

int actualiserMap(void) {
    for (unsigned i = 0; i < DIMENSION_MAP; ++i) {
        for (unsigned j = 0; j < DIMENSION_MAP; ++j) {
            if (i == perso.posX && j == perso.posY) {
                map[i][j].contenu = 1;
            } else if (i == persoPast.posX && j == persoPast.posY) {
                map[i][j].contenu = 0;
            }
        }
    }

    return EXIT_SUCCESS;
}

unsigned int aleatoire(int salle, int graine, int min, int max){
    double rdn;
    max++;

    rdn = (cos(salle + graine) + 1) / 2; // Valeur entre 0 et 1
    rdn = rdn * (max - min) + min; // Réajustement de la plage
    return (unsigned int)rdn;
}

int nouvelleSalle(int longueur, int largeur, int num_salle, int cote) {
    char str[100];

    // Réinitialisation room
    room.num = 0;
    room.largeur = 0;
    room.longueur = 0;
    room.posX = 0;
    room.posY = 0;

    if (room.cases != NULL) {
        freeRoomCases(room.cases, room.largeur); // Utiliser la fonction de libération
        room.cases = NULL;
    }

    debug("1");

    // Génération de la nouvelle salle
    if (generation(longueur, largeur, num_salle, cote) != EXIT_SUCCESS) {
        printf("Erreur generation\n");
        return EXIT_FAILURE;
    }

    debug("2");

    // Calcul de la position de la salle
    if (num_salle == 1) {
        room.posX = perso.posX - 2;
        room.posY = perso.posY - 2;
    } 
    else {
        switch (cote) {
            case 0: // Haut
                room.posX = perso.posX - entreeX;
                room.posY = perso.posY - longueur;
                break;
            case 3: // Droite
                room.posX = perso.posX + 1;
                room.posY = perso.posY - entreeY;
                break;
            case 2: // Bas
                room.posX = perso.posX - entreeX;
                room.posY = perso.posY + 1;
                break;
            case 1: // Gauche  
                room.posX = perso.posX - largeur;
                room.posY = perso.posY - entreeY;
                break;
            default:
                break;
        }
    }
    sprintf(str, "cote /entreeX / Y: %d / %d|%d", cote, entreeX, entreeY);
    debug(str);
    getch();
    debug("3");

    // Vérification des limites de la carte
    if (room.posX < 0 || room.posX + room.largeur >= DIMENSION_MAP || room.posY < 0 || room.posY + room.longueur >= DIMENSION_MAP) {
        map[perso.posX][perso.posY].contenu = -1;

        switch (cote) {
            case 0:
                mouvementBas();
                break;
            case 1:
                mouvementDroite();
                break;
            case 2:
                mouvementHaut();
                break;
            case 3:
                mouvementGauche();
                break;
            default:
                break;
        }
    }

    debug("4");

    // Ajout de la salle à la carte
    ajouterSalle();
    map[perso.posX][perso.posY].contenu = 1;

    return EXIT_SUCCESS;
}

int generation(int longueur, int largeur, int num_salle, int cote) {
    // Allocation de mémoire pour la salle
    char str[100];

    tile **p = malloc(sizeof(tile *) * largeur);
    if (p == NULL) {
        printf("Echec de l'allocation\n");
        return EXIT_FAILURE;
    }

    for (int i = 0; i < largeur; i++) {
        p[i] = malloc(sizeof(tile) * longueur);
        if (p[i] == NULL) {
            printf("Echec de l'allocation\n");
            // Libération de la mémoire allouée
            for (int j = 0; j < i; j++) {
                free(p[j]);
            }
            free(p);
            return EXIT_FAILURE;
        }
    }

    // Initialisation des cases
    for (int i = 0; i < largeur; i++) {
        for (int j = 0; j < longueur; j++) {
            p[i][j].contenu = 0;
            p[i][j].mstr.hp = 0;
            p[i][j].mstr.xp = 0;
            p[i][j].mstr.loot = 0;
            p[i][j].mstr.frameAnimation = 0;
            p[i][j].spe.type = 0;
            p[i][j].spe.inv = 0;
        }
    }

    // Initialisation des portes
    int porteLibre = 0;
    int portes[4] = {0}; // Initialiser toutes les portes à 0

    // Si c'est la première salle, positionner les portes au centre des murs
    if (num_salle == 1) {
        portes[0] = 1;
        portes[1] = 1;
        portes[2] = 1;
        portes[3] = 1;
    } else {
        portes[cote] = 1; // Activer la porte du côté spécifié

        for (int i = 0; i < 4; i++) {
            if (portes[i] == 0 && aleatoire(num_salle, i, 1, 100) <= 60) {
                portes[i] = 1;
                porteLibre++;
            }
        }
    }

    for (int i = 0; i < 4; i++) {
        if (portes[i] == 1) {
            int al;
            switch (i) {
                case 0: // Porte haut
                    al = (num_salle == 1) ? longueur / 2 : aleatoire(num_salle, i, 1, longueur - 2);
                    p[0][al].contenu = -1;
                    if (i == cote) {
                        entreeX = al;
                        entreeY = 0;
                    }
                    break;
                case 1: // Porte gauche
                    al = (num_salle == 1) ? largeur / 2 : aleatoire(num_salle, i, 1, largeur - 2);
                    p[al][0].contenu = -1;
                    if (i == cote) {
                        entreeX = 0;
                        entreeY = al;
                    }
                    break;
                case 2: // Porte bas
                    al = (num_salle == 1) ? longueur / 2 : aleatoire(num_salle, i, 1, longueur - 2);
                    p[largeur - 1][al].contenu = -1;
                    if (i == cote) {
                        entreeX = al;
                        entreeY = longueur - 1;
                    }
                    break;
                case 3: // Porte droite
                    al = (num_salle == 1) ? largeur / 2 : aleatoire(num_salle, i, 1, largeur - 2);
                    p[al][longueur - 1].contenu = -1;
                    if (i == cote) {
                        entreeX = largeur - 1;
                        entreeY = al;
                    }
                    break;
                default:
                    break;
            }
        }
    }

    int obj = 0;
    for (int i = 0; i < largeur; i++) {
        for (int j = 0; j < longueur; j++) {
            if (j == 0 || j == longueur - 1 || i == 0 || i == largeur - 1) {
                if (p[i][j].contenu != -1) {
                    p[i][j].contenu = -2;
                }
            } else if (num_salle == 1) {
                p[2][2].contenu = 1;
            } else if (obj < ((longueur - 2) * (largeur - 2)) / OBJ_MAX) {
                if (aleatoire(num_salle, i * j, 1, 100) <= 40) {
                    int al = aleatoire(num_salle, i * j, 1, 10);
                    if (al <= 3) {
                        p[i][j].contenu = 2;
                        p[i][j].mstr.hp = 80;
                        p[i][j].mstr.xp = 10;
                    } else if (al <= 5) {
                        p[i][j].contenu = 2;
                        p[i][j].mstr.hp = 180;
                        p[i][j].mstr.xp = 20;
                    } else if (al == 6) {
                        p[i][j].contenu = 2;
                        p[i][j].mstr.hp = 260;
                        p[i][j].mstr.xp = 30;
                    } else if (al <= 8) {
                        p[i][j].contenu = 3;
                        p[i][j].spe.type = 2;
                    } else if (al == 9) {
                        p[i][j].contenu = 3;
                        p[i][j].spe.type = 3;
                    } else {
                        p[i][j].contenu = 3;
                        p[i][j].spe.type = 1;
                    }
                    obj++;
                }
            }
        }
    }

    sprintf(str, "cote /entreeX / Y: %d / %d|%d", cote, entreeX, entreeY);
    debug(str);
    getch();

    room.num = num_salle;
    room.largeur = largeur;
    room.longueur = longueur;
    room.cases = p;

    return EXIT_SUCCESS;
}


void ajouterSalle(void) {
    int n = 0, m = 0;
    char str[100];
    sprintf(str, "Ajout de la salle: %d, %d | %d, %d", room.posX, room.posX + room.largeur, room.posY, room.posY + room.longueur);
    debug(str);

    for (unsigned int i = room.posX; i < room.posX + room.largeur; i++) {
        for (unsigned int j = room.posY; j < room.posY + room.longueur; j++) {
            if (i >= DIMENSION_MAP || j >= DIMENSION_MAP) {
                sprintf(str, "Accès hors limites: %d, %d", i, j);
                debug(str);
                continue; // Ne pas accéder à la mémoire hors limites
            }

            map[i][j].contenu = room.cases[n][m].contenu;
            map[i][j].mstr.hp = room.cases[n][m].mstr.hp;
            map[i][j].mstr.xp = room.cases[n][m].mstr.xp;
            map[i][j].mstr.loot = room.cases[n][m].mstr.loot;
            map[i][j].mstr.frameAnimation = room.cases[n][m].mstr.frameAnimation;
            map[i][j].spe.type = room.cases[n][m].spe.type;
            map[i][j].spe.inv = room.cases[n][m].spe.inv;

            sprintf(str, "Copie de la salle: %d, %d | %d, %d", i, j, map[i][j].contenu, room.cases[n][m].contenu);
            debug(str);

            m++;
        }
        m = 0;
        n++;
    }
}