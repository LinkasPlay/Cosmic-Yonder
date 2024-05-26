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

// COMMANDE TERMINAL : gcc -o ProgMain *.c -lncurses -lm -lpthread -lpulse-simple -lpulse

extern int graine;
int entreeX;
int entreeY;

extern salle room;

void ajouterSalle (void);

unsigned int aleatoire(int salle, int graine, int min, int max){
    double rdn;
    max++;

    rdn = (cos(salle + graine) + 1) / 2; // Valeur entre 0 et 1
    rdn = rdn * (max - min) + min; // Réajustement de la plage
    return (unsigned int)rdn;
}

int nouvelleSalle(int longueur, int largeur, int num_salle, int cote) {
    // Réinitialisation room
    room.num = 0;
    room.largeur = 0;
    room.longueur = 0;
    room.posX = 0;
    room.posY = 0;
    if (room.cases != NULL) {
        free(room.cases); // Libérer la mémoire si nécessaire
        room.cases = NULL;
    }

    // Génération de la nouvelle salle
    if (generation(longueur, largeur, num_salle, cote) != EXIT_SUCCESS) {
        printf("Erreur generation\n");
        return EXIT_FAILURE;
    }

    // Calcul de la position de la salle
    if (num_salle == 1) {
        room.posX = perso.posX - 2;
        room.posY = perso.posY - 2;
    } else {
        switch (cote) {
            case 0:
                room.posX = perso.posX - 2;
                room.posY = perso.posY - longueur;
                break;
            case 1:
                room.posX = perso.posX - largeur;
                room.posY = perso.posY - 2;
                break;
            case 2:
                room.posX = perso.posX - 2;
                room.posY = perso.posY;
                break;
            case 3:
                room.posX = perso.posX;
                room.posY = perso.posY - 2;
                break;
            default:
                break;
        }
    }

    // Vérification des limites de la carte
    if (room.posX < 0 || room.posX + room.largeur >= DIMENSION_MAP || room.posY < 0 || room.posY + room.longueur >= DIMENSION_MAP) {
        map[perso.posX][perso.posY].contenu = -1; // Assumant que `contenu` est un int

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

    // Ajout de la salle à la carte
    ajouterSalle();
    map[perso.posX][perso.posY].contenu = 1; // Assumant que `contenu` est un int

    return EXIT_SUCCESS;
}

int generation(int longueur, int largeur, int num_salle, int cote){

    tile **p = malloc(sizeof(tile *) * largeur);
    if (p == NULL) {
        printf("Echec de l'allocation\n");
        return EXIT_FAILURE;
    }
    
    for (unsigned i = 0; i < largeur; i++) {
        p[i] = malloc(sizeof(tile) * longueur);
        
        if (p[i] == NULL) {
            printf("Echec de l'allocation\n");
            // Free previously allocated memory to prevent memory leaks
            for (unsigned j = 0; j < i; j++) {
                free(p[j]);
            }
            free(p);
            return EXIT_FAILURE;
        }
    }

    // Initialize the tiles to some default state
    for (unsigned i = 0; i < largeur; i++) {
        for (unsigned j = 0; j < longueur; j++) {
            p[i][j].contenu = 0;
            p[i][j].mstr.hp = 0;
            p[i][j].mstr.xp = 0;
            p[i][j].mstr.loot = 0;
            p[i][j].mstr.frameAnimation = 0;
            p[i][j].spe.type = 0;
            p[i][j].spe.inv = 0;
        }
    }

    int porteCoord[4];
    int al = 1;
    int portes[4];

    
    if (porteLibre == 1){
        portes[0] = 1;
        portes[1] = 1;
        portes[2] = 1;
        portes[3] = 1;
    }
    portes[cote] = -1;
    
    for(unsigned i=0 ; i<4 ; i++){
        if (num_salle == 1){
            porteLibre = 4;
            p[0][2].contenu = -1;
            p[0][2].spe.type = 0;

            p[4][2].contenu = -1;
            p[4][2].spe.type = 0;  

            p[2][0].contenu = -1;
            p[2][0].spe.type = 0;  

            p[2][4].contenu = -1;
            p[2][4].spe.type = 0;  
        }
        else{
            if (portes[i] == 0){
                if(aleatoire(i * num_salle, graine * i - num_salle, 1, 100)<=60){
                    portes[i] = 1;
                    porteLibre ++;
                }
                else{
                    portes[i] = 0;
                }
                
                
            }
            if (portes[i] == 1){
                switch (i){

                case 0:
                    al = aleatoire(i * num_salle, graine * 6, 1, largeur-2);
                    p[0][al].contenu = -1;
                    p[0][al].spe.type = 0;
                    if(i == cote){
                        entreeX = 0;
                        entreeY = al;
                    }
                    break;

                case 1:
                    al = aleatoire(i * num_salle, graine * 4, 1, longueur-2);
                    p[al][0].contenu = -1;
                    p[al][0].spe.type = 1;
                    if(i == cote){
                        entreeX = al;
                        entreeY = 0;
                    }
                    break;

                case 2:
                    al = aleatoire(i * num_salle, graine * 7, 1, largeur-2);
                    p[longueur-1][al].contenu = -1;
                    p[longueur-1][al].spe.type = 2;
                    if(i == cote){
                        entreeX = longueur-1;
                        entreeY = al;
                    }
                    break;

                case 3:
                    al = aleatoire(i * num_salle, graine * 2, 1, longueur-2);
                    p[al][largeur-1].contenu = -1;
                    p[al][largeur-1].spe.type = 3;
                    if(i == cote){
                        entreeX = al;
                        entreeY = largeur-1;
                    }
                    break;
                
                default:
                    break;
                }
                
            }
        }
    }

    porteLibre--;

    int obj = 0;

    for(unsigned i = 0 ; i<largeur ; i++){
        for(unsigned j = 0 ; j<largeur ; j++){
            if(j==0 || j==largeur-1 || i==0 || i==longueur-1){
                if (p[i][j].contenu != -1){
                    p[i][j].contenu = -2;
                }

                p[i][j].mstr.hp = 0;
                p[i][j].mstr.xp = 0;
                p[i][j].mstr.loot = 0;
                p[i][j].mstr.frameAnimation = 0;

                p[i][j].spe.inv = 0;

            }
            else if (num_salle == 1){
                p[i][j].contenu == 0;
                p[2][2].contenu == 1;
            }
            else if (obj < ( ( (longueur - 2) * (largeur - 2) ) / OBJ_MAX ) ){

                if(aleatoire(num_salle, graine, 1, 100)<=40){

                    //placement objet
                    al = aleatoire(num_salle, graine * 17, 1, 10);
                    switch (al)
                    {

                    // monstre niv 1 (30%)
                    case 1:
                    case 2:
                    case 3:
                        p[i][j].contenu = 2;
                        p[i][j].mstr.hp = 80;
                        p[i][j].mstr.xp = 10;
                        p[i][j].mstr.loot = 0; //aleatoire
                        p[i][j].mstr.frameAnimation = 0;
                    
                    // monstre niv 2 (20%)
                    case 4:
                    case 5:
                   
                        p[i][j].contenu=2;
                        p[i][j].mstr.hp=180;
                        p[i][j].mstr.xp = 20;
                        p[i][j].mstr.loot = 0; //aleatoire 
                        p[i][j].mstr.frameAnimation = 0;
                    
                    // monstre niv 3 (10%)
                    case 6:
                        p[i][j].contenu=2;
                        p[i][j].mstr.hp=260;
                        p[i][j].mstr.xp=30;
                        p[i][j].mstr.loot=0; 
                        p[i][j].mstr.frameAnimation=0;
                    
                    case 7:                    
                    case 8:
                        p[i][j].contenu=3;
                        p[i][j].spe.type=2;
                    case 9:
                        p[i][j].contenu=3;
                        p[i][j].spe.type=3;    
                    
                    case 10:
                        p[i][j].contenu=3;
                        p[i][j].spe.type=1;

                        break;
                    
                    default:
                        break;
                    }

                }
                else {

                    p[i][j].contenu = 0;
                    p[i][j].mstr.hp = 0;
                    p[i][j].mstr.xp = 0;
                    p[i][j].mstr.loot = 0;
                    p[i][j].mstr.frameAnimation = 0;
                    p[i][j].spe.type = 0;
                    p[i][j].spe.inv = 0;

                }
            }
            else {
                
                p[i][j].contenu = 0;
                p[i][j].mstr.hp = 0;
                p[i][j].mstr.xp = 0;
                p[i][j].mstr.loot = 0;
                p[i][j].mstr.frameAnimation = 0;
                p[i][j].spe.type = 0;
                p[i][j].spe.inv = 0;
                
            }
            
        }
    }
    
    room.num = num_salle;
    room.largeur = largeur;
    room.longueur = longueur;
    room.cases = p;
    return EXIT_SUCCESS;
}   


void ajouterSalle(void) {
    int n = 0, m = 0;
    char str[20];
    sprintf(str, "%d, %d | %d, %d", room.posX, room.posX + room.largeur, room.posY, room.posY + room.longueur);
    debug(str);

    for (unsigned int i = room.posX; i < room.posX + room.largeur; i++) {
        for (unsigned int j = room.posY; j < room.posY + room.longueur; j++) {
            map[i][j].contenu = room.cases[n][m].contenu;
            map[i][j].mstr.hp = room.cases[n][m].mstr.hp;
            map[i][j].mstr.xp = room.cases[n][m].mstr.xp;
            map[i][j].mstr.loot = room.cases[n][m].mstr.loot;
            map[i][j].mstr.frameAnimation = room.cases[n][m].mstr.frameAnimation;
            map[i][j].spe.type = room.cases[n][m].spe.type;
            map[i][j].spe.inv = room.cases[n][m].spe.inv;

            sprintf(str, "%d, %d | %d, %d", i, j, map[i][j].contenu, room.cases[n][m].contenu);
            debug(str);

            m++;
        }
        m = 0;
        n++;
    }
}

//test