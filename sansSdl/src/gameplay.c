#include "CosmicYonder.h"
#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <pulse/simple.h>
#include <pulse/error.h>
#include <time.h>
// COMMANDE TERMINAL : gcc -o ProgMain *.c -lncurses -lm -lpthread -lpulse-simple -lpulse

/*
Expliquation gameplay du jeu :

    on commence avec l'invetaire vide
    1) l'épée/tuyau permet de frapper autour de soi, fait 50 de dégats
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
    il faut réparé 3 grandes machines pour avoir la vrai fin

    il y a quatre type de monstres :
        niv 1 : 80 pv, 10 xp
        niv 2 : 180 pv, 20 xp | drop : clé * 0.05
        niv 3 : 260 pv, 30 xp | drop : clé * 0.1, clé a molette * 0.1
        niv 5 (boss) : 1000 pv, 0 xp => fin du jeu

*/

extern int texture( int argc, char **argv);
extern int creeMap(void);
extern int actualiserMap(void);
extern int afficher_image_ascii(WINDOW *win, const char *filename);
extern void print_menu(WINDOW *menu_win, int highlight, int n_choices, char *choices[]);
extern int nouvelleSalle(int longueur, int largeur, int num_salle, int cote);
extern unsigned int aleatoire(int salle, int graine, int min, int max);
extern void save_game(const char *filename);

extern tile **map;
extern int graine;

extern int Xcamera;
extern int Ycamera;

extern personnage perso;
extern personnage persoPast;

int num_salle;

void lvlMaj(monstre mstr){
    perso.xp = perso.xp + mstr.xp;
    if (perso.xp > ( 100 * ( perso.lvl * 0.9 ) ) ){
        perso.xp = perso.xp - ( 100 * ( perso.lvl * 0.9 ) );
        perso.lvl ++;
    }
}

int testSol(int x, int y,int cote){
    switch (map[x][y].contenu){

    case -2:
    case 3:
        return 0;
        break;

    case -1:
        num_salle ++;
        int largeur = aleatoire(15 * num_salle, graine * 7, 5, 11);
        int longueur = aleatoire(9 * num_salle, graine * 2, 5, 11);
        nouvelleSalle(longueur, largeur, num_salle, cote);
        return 1;
        break;

    case 2:
        return 2;
        break;
    
    default:
        return 1;
        break;
    }
   return 1;
}

int degatMonstre(int dmg, monstre mstr){
    mstr.hp = mstr.hp - dmg;
    // monstre seulement blessé
    if (mstr.hp > 0){
        mstr.frameAnimation = -5;
    }
    // monstre tué
    else {
        mstr.frameAnimation = -10;
        lvlMaj(mstr);
        if (mstr.loot > 1){
            perso.inv[mstr.loot] ++;
        }
    }
}

int monstreMouvement(int x, int y){
    int x2 = x, y2 = y;
    for(unsigned i = x-2; i < x+3; i++){
        for(unsigned j = y-2; i < y+3; i++){
            if (map[i][j].contenu == 1){
                if (i <= x){
                    x2 = x - 1;
                }
                else if (i > x){
                    x2 = x + 1;
                }
                else if (j < y){
                    y2 = y - 1;
                }
                else{
                    y2 = y + 1;
                }
                map[x2][y2].contenu = map[x][y].contenu;
                map[x2][y2].mstr = map[x][y].mstr;
                map[x][y].contenu = 0;
                map[x][y].mstr.frameAnimation = 0;
                map[x][y].mstr.hp = 0;
                map[x][y].mstr.xp = 0;
                map[x][y].mstr.loot = 0;
                return EXIT_SUCCESS;
            }
        }
    }
    
    return EXIT_SUCCESS;


}

int attaqueEpee(void){

    switch (perso.direction) {
        
        //vers le haut
        case 1 :
            // annimation a ajouté ici
            if (map[perso.posX - 1][perso.posY].contenu == 2){
                degatMonstre(perso.lvl * 10 + 40, map[perso.posX - 1][perso.posY].mstr);
            }
            if (map[perso.posX - 1][perso.posY - 1].contenu == 2){
                degatMonstre(perso.lvl * 10 + 40, map[perso.posX - 1][perso.posY - 1].mstr);
            }
            if (map[perso.posX ][perso.posY - 1].contenu == 2){
                degatMonstre(perso.lvl * 10 + 40, map[perso.posX][perso.posY - 1].mstr);
            }
            if (map[perso.posX + 1][perso.posY - 1].contenu == 2){
                degatMonstre(perso.lvl * 10 + 40, map[perso.posX + 1][perso.posY - 1].mstr);
            }
            if (map[perso.posX + 1][perso.posY].contenu == 2){
                degatMonstre(perso.lvl * 10 + 40, map[perso.posX + 1][perso.posY].mstr);
            }
            break;

        //vers la gauche
        case 2 :
            // annimation a ajouté ici
            if (map[perso.posX][perso.posY + 1].contenu == 2){
                degatMonstre(perso.lvl * 10 + 40, map[perso.posX][perso.posY + 1].mstr);
            }
            if (map[perso.posX - 1][perso.posY + 1].contenu == 2){
                degatMonstre(perso.lvl * 10 + 40, map[perso.posX - 1][perso.posY + 1].mstr);
            }
            if (map[perso.posX - 1][perso.posY].contenu == 2){
                degatMonstre(perso.lvl * 10 + 40, map[perso.posX + 1][perso.posY].mstr);
            }
            if (map[perso.posX - 1][perso.posY - 1].contenu == 2){
                degatMonstre(perso.lvl * 10 + 40, map[perso.posX - 1][perso.posY - 1].mstr);
            }
            if (map[perso.posX][perso.posY - 1].contenu == 2){
                degatMonstre(perso.lvl * 10 + 40, map[perso.posX][perso.posY - 1].mstr);
            }
            break;

        //vers le bas
        case 3 :
            // annimation a ajouté ici
            if (map[perso.posX + 1][perso.posY].contenu == 2){
                degatMonstre(perso.lvl * 10 + 40, map[perso.posX + 1][perso.posY].mstr);
            }
            if (map[perso.posX + 1][perso.posY + 1].contenu == 2){
                degatMonstre(perso.lvl * 10 + 40, map[perso.posX + 1][perso.posY + 1].mstr);
            }
            if (map[perso.posX ][perso.posY + 1].contenu == 2){
                degatMonstre(perso.lvl * 10 + 40, map[perso.posX][perso.posY + 1].mstr);
            }
            if (map[perso.posX - 1][perso.posY + 1].contenu == 2){
                degatMonstre(perso.lvl * 10 + 40, map[perso.posX - 1][perso.posY + 1].mstr);
            }
            if (map[perso.posX - 1][perso.posY].contenu == 2){
                degatMonstre(perso.lvl * 10 + 40, map[perso.posX - 1][perso.posY].mstr);
            }
            break;

        //vers la doite
        case 4 :
            // annimation a ajouté ici
            if (map[perso.posX][perso.posY - 1].contenu == 2){
                degatMonstre(perso.lvl * 10 + 40, map[perso.posX][perso.posY - 1].mstr);
            }
            if (map[perso.posX + 1][perso.posY - 1].contenu == 2){
                degatMonstre(perso.lvl * 10 + 40, map[perso.posX + 1][perso.posY - 1].mstr);
            }
            if (map[perso.posX + 1][perso.posY].contenu == 2){
                degatMonstre(perso.lvl * 10 + 40, map[perso.posX + 1][perso.posY].mstr);
            }
            if (map[perso.posX + 1][perso.posY + 1].contenu == 2){
                degatMonstre(perso.lvl * 10 + 40, map[perso.posX + 1][perso.posY + 1].mstr);
            }
            if (map[perso.posX][perso.posY + 1].contenu == 2){
                degatMonstre(perso.lvl * 10 + 40, map[perso.posX][perso.posY + 1].mstr);
            }
            break;

    }

    return EXIT_SUCCESS;
}

int mouvementHaut(void){
    switch (testSol(perso.posX, perso.posY - 1, 2)) {
    case 1:
    case -1:
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
        break;

    case 2:
        //degats
        break;
    
    default:
        break;
    }
}

int mouvementGauche(void){
    switch (testSol(perso.posX - 1, perso.posY, 3)) {
        case 1:
        case -1:
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

        case 2:

            break;
    
        default:
            break;
    }
}

int mouvementBas(void){
    switch (testSol(perso.posX, perso.posY + 1, 0)) {
        case 1:
        case -1:
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

        case 2:

            break;
    
        default:
            break;
    }
}

int mouvementDroite(void){
    switch (testSol(perso.posX + 1, perso.posY, 1)) {
        case 1:
        case -1:
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

        case 2:

            break;
    
        default:
            break;
    }
}

char *choicepause[] = { 
    "Reprendre la partie",
    "Sauvegarder",
    "Option",
    "Quitter la partie",
};

int n_choicepause = 4;

int pause(){
    //Création du menu pause
    endwin();
    
    WINDOW *menu_pause;
    int highlight1 = 1;
    int choice2 = 0;
    int c2 = 0;

    int LINES2 = getmaxy(stdscr);
    int COLS2 = getmaxx(stdscr);

    int winHauteur2 = (int)(LINES2 * 0.3);
    int winLargeur2 = (int)(COLS2 * 0.6);

    int titleY2 = ((LINES2 - 5)/6);
    int titleX2 = (COLS2 - 71) / 2;

    int winY2 = ((LINES2 - winHauteur2) / 4) * 3;
    int winX2 = (COLS2 - winLargeur2) / 2;
    WINDOW *title2 = newwin(5, 71, titleY2, titleX2);

    if(afficher_image_ascii(title2, "image/pause.txt") != EXIT_SUCCESS){

        printf("Erreur");
        exit(EXIT_FAILURE);
    }
    wrefresh(title2);

    menu_pause = newwin(winHauteur2, winLargeur2, winY2, winX2);
    keypad(menu_pause, TRUE);
    refresh();
    print_menu(menu_pause, highlight1, n_choicepause, choicepause);

    while (1) {
        // Check for terminal resize
        int new_LINES2 = getmaxy(stdscr);
        int new_COLS2 = getmaxx(stdscr);
        if (new_LINES2 != LINES2 || new_COLS2 != COLS2) {
            LINES2 = new_LINES2;
            COLS2 = new_COLS2;

            winHauteur2 = (int)(LINES2 * 0.3);
            winLargeur2 = (int)(COLS2 * 0.6);

            winY2 = (LINES2 - winHauteur2) / 2;
            winX2 = (COLS2 - winLargeur2) / 2;
            titleY2 = ((LINES2 - 5)/6);
            titleX2 = (COLS2 - 71) / 2;

            clear();
            refresh();

            // Delete the old window and create a new one
            delwin(menu_pause);
            menu_pause = newwin(winHauteur2, winLargeur2, winY2, winX2);
            title2 = newwin(5, 71, titleY2, titleX2);
            if(afficher_image_ascii(title2, "image/pause.txt") != EXIT_SUCCESS){

                printf("Erreur");
                exit(EXIT_FAILURE);
            }
            wrefresh(title2);
            keypad(menu_pause, TRUE);
            refresh();
        }

        print_menu(menu_pause, highlight1, n_choicepause, choicepause);
        c2 = wgetch(menu_pause);
        switch (c2) {
            case KEY_UP:
                if (highlight1 == 1)
                    highlight1 = n_choicepause;
                else
                    --highlight1;
                break;
            case KEY_DOWN:
                if (highlight1 == n_choicepause)
                    highlight1 = 1;
                else
                    ++highlight1;
                break;
            case 10: // Enter key
                choice2 = highlight1;
                break;
            default:
                refresh();
                break;
        }
        if (choice2 != 0) // User did a choice come out of the infinite loop
            break;
    }

    clrtoeol();
    refresh();
    endwin();
    
    switch (choice2) {
        case 1:
            return EXIT_SUCCESS;
            break;
        case 2:
            save_game("Fichierdesauvegarde.txt");
            // mettre un texte pour dire si la sauvegarde s'est lancé ou pas
            return EXIT_SUCCESS;
            break;
            
        case 4:
            printf("Fin du jeu\n");
            endwin();
            return -1;
            break;
        
        default:
            break;
    }
    return EXIT_SUCCESS;
}

// Structure pour les arguments du timer
typedef struct {
    int minutes;
} TimerArgs;

// Fonction du thread du timer
void* timer_thread(void* args) {
    TimerArgs* timer_args = (TimerArgs*)args;
    int minutes = timer_args->minutes;
    
    time_t start_time = time(NULL);
    time_t end_time = start_time + minutes * 60;

    printf("Le timer de %d minutes commence maintenant...\n", minutes);

    while (time(NULL) < end_time) {
        int remaining_seconds = end_time - time(NULL);
        int remaining_minutes = remaining_seconds / 60;
        remaining_seconds %= 60;

        printf("\rTemps restant: %02d:%02d", remaining_minutes, remaining_seconds);
        fflush(stdout);

        sleep(1);
    }

    printf("\nLe timer de %d minutes est écoulé !\n", minutes);
    return NULL;
}

// Fonction pour démarrer le timer



void start_timer(int *minutes) {
    pthread_t timer_tid;

    // Créer le thread du timer en mode détaché
    if (pthread_create(&timer_tid, NULL, timer_thread, minutes) != 0) {
        fprintf(stderr, "Erreur de création du thread du timer\n");
        return;
    }

    // Définir le thread comme détaché
    if (pthread_detach(timer_tid) != 0) {
        fprintf(stderr, "Erreur de définition du thread du timer comme détaché\n");
        return;
    }
}

