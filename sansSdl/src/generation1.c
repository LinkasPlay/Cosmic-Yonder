#include <stdlib.h>
#include <unistd.h>

#include <ncurses.h>

#include "texture.h"
#include <math.h>

#define OBJ_MAX 9

#define LARGEUR_TAB 5
#define LONGUEUR_TAB 5

#define LIGNES 5
#define COLONNES 5

int porteLibre = 0;

// COMMANDE TERMINAL : gcc -o ProgMain *.c -lncurses -lm -lpthread -lpulse-simple -lpulse


//message erreur

/*
void erreur(char message){
	printf("ERREUR : %s\n",message);
	exit(0);
}

int creeTab(void) {

    int largeurTab, longueurTab;
    printf("Quelle est la largeur du carré ?");
    scanf("%d",(&largeurTab) - 1);
    printf("Quelle est la longueur du carré ?");
    scanf("%d",(&longueurTab) - 1);

    int **p = malloc(sizeof(int*[largeurTab]));

    if (p == NULL) {
        printf("Échec de l'allocation\n");
        return EXIT_FAILURE;
    }

    for (unsigned i = 0; i < longueurTab; ++i) {
        p[i] = malloc(sizeof(int[longueurTab]));

        if (p[i] == NULL) {
            printf("Échec de l'allocation\n");
            return EXIT_FAILURE;
        }
    }

    for (unsigned i = 0; i < largeurTab; ++i) {
        for (unsigned j = 0; j < longueurTab; ++j) {
            p[i][j] = (i * longueurTab) + j;
            printf("p[%u][%u] = %d\n", i, j, p[i][j]);
        }
    }

    for (unsigned i = 0; i < 3; ++i) {
        free(p[i]);
    }

    free(p);
    return 0;
}

/*
J'ai fais un programme qui va creer un tableau de 5 par 5 ou chaque element va etre remplacer par une image.
Aussi la case au centre tout en haut est une porte pour acceder au niveau suivant.
*/



int graine=69; 

extern salle room;

unsigned int aleatoire(int salle, int graine, int min, int max){
    double rdn;
    max++;

    rdn = (cos(salle + graine) + 1) / 2; // Valeur entre 0 et 1
    rdn = rdn * (max - min) + min; // Réajustement de la plage
    return (unsigned int)rdn;
}

int generation(int longueur, int largeur, int num_salle, int cote){

    tile **p = malloc(sizeof(tile *)*largeur);
    if(p==NULL){
        printf("Echec de l'allocation\n");
        return EXIT_FAILURE;
    }
    
    for(unsigned i = 0 ; i<largeur ; i++){
        p[i]=malloc(sizeof(tile)*longueur);
        
        if(p[i]==NULL){
            printf("Echec de l'allocation\n");
            return EXIT_FAILURE;
        }
    }

    int porteCoord[4];
    int al = 1;
    int portes[4];

    portes[cote] = 1;
    if (porteLibre == 1){
        portes[0] = 1;
        portes[1] = 1;
        portes[2] = 1;
        portes[3] = 1;
    }
    
    for(unsigned i=0 ; i<4 ; i++){
        printf("\ni = %d \n", i);
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
            if (portes[i] != 1){
                if(aleatoire(i * num_salle, graine * i - num_salle, 1, 100)<=60){
                    portes[i] = 1;
                    porteLibre ++;
                }
                else{
                    portes[i] = 0;
                }
                
                
            }
            printf("portes %d = %d \n", i, portes[i]);
            if (portes[i] == 1){
                switch (i){

                case 0:
                    al = aleatoire(i * num_salle, graine * 6, 1, largeur-2);
                    p[0][al].contenu = -1;
                    p[0][al].spe.type = 0;
                    break;

                case 1:
                    al = aleatoire(i * num_salle, graine * 4, 1, longueur-2);
                    p[al][0].contenu = -1;
                    p[al][0].spe.type = 1;
                    break;

                case 2:
                    al = aleatoire(i * num_salle, graine * 7, 1, largeur-2);
                    p[longueur-1][al].contenu = -1;
                    p[longueur-1][al].spe.type = 2;
                    break;

                case 3:
                    al = aleatoire(i * num_salle, graine * 2, 1, longueur-2);
                    p[al][largeur-1].contenu = -1;
                    p[al][largeur-1].spe.type = 3;
                    break;
                
                default:
                    break;
                }
                printf("\nal = %d (nsal = %d, gr = %d, la-2 = %d \n\n", al, num_salle, graine, largeur-2);
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
            }
            
        }
    }
    
    room.num = num_salle;
    room.largeur = largeur;
    room.longueur = longueur;
    room.posX = 10;
    room.posY = 10;
    room.cases = p;
    return EXIT_SUCCESS;
}   

