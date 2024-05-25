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

int porteLibre = 0;
extern personnage perso;

// COMMANDE TERMINAL : gcc -o ProgMain *.c -lncurses -lm -lpthread -lpulse-simple -lpulse

int graine=69; 
int entreeX;
int entreeY;

extern salle room;

unsigned int aleatoire(int salle, int graine, int min, int max){
    double rdn;
    max++;

    rdn = (cos(salle + graine) + 1) / 2; // Valeur entre 0 et 1
    rdn = rdn * (max - min) + min; // Réajustement de la plage
    return (unsigned int)rdn;
}

int nouvelleSalle(int longueur, int largeur, int num_salle, int cote){

    //réinitialisation room
    room.num = 0;
    room.largeur = 0;
    room.longueur = 0;
    room.posX = 0;
    room.posY = 0;
    room.cases = NULL;

    if (generation(longueur, largeur, num_salle, cote) != EXIT_SUCCESS) {
        printf("Erreur generation");
        return EXIT_FAILURE;
    }

    room.posX = (perso.posX - entreeX);
    room.posY = (perso.posY - entreeY);

    if (room.posX < 0 || room.posX >= DIMENSION_MAP - room.largeur || room.posY < 0 || room.posY >= DIMENSION_MAP - room.longueur){

        map[perso.posX][perso.posY].contenu = -1;

        switch (cote){
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

    return EXIT_SUCCESS;

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

    portes[cote] = -1;
    if (porteLibre == 1){
        portes[0] = 1;
        portes[1] = 1;
        portes[2] = 1;
        portes[3] = 1;
    }
    
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
            printf("portes %d = %d \n", i, portes[i]);
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
            }
            
        }
    }
    
    room.num = num_salle;
    room.largeur = largeur;
    room.longueur = longueur;
    room.cases = p;
    return EXIT_SUCCESS;
}   


int ajouterSalle (void){
    for(unsigned int i = room.posX; i <= room.largeur; i++){
        for(unsigned int i = room.posX; i <= room.largeur; i++){
        
        }
    }
}