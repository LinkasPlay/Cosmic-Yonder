#ifndef COSMICYONDER_H
#define COSMICYONDER_H

#define DIMENSION_MAP 300
#define WINDOW_WIDTH 1500
#define WINDOW_HEIGHT 900

#define CASE_WIDTH 18
#define CASE_HEIGHT 7

#define FPS_LIMIT 16

// COMMANDE TERMINAL : gcc -o ProgMain *.c -lncurses -lm -lpthread -lpulse-simple -lpulse

extern void SDL_ExitWithError(const char *message);
//extern int texture(WINDOW *win);
/*
int contenuCase = 0;
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
SDL_Rect Case;
*/

typedef struct personnage {
    int direction; //haut = 1, gauche = 2, bas = 3, droite = 4
    int posX;
    int posY;
    int frameAnimation; // 4 frames , -5 = dégat, -10 = dcd
    int hp;
    int xp;
    int lvl;
    int * inv;
} personnage;

typedef struct monstre {
    int hp;
    int xp;
    int loot;
    int frameAnimation; // 4 frames , -5 = dégat, -10 = dcd
} monstre;

typedef struct special {
    int type; // 1 = coffre; 2 = machine, 3 = grande machine, 4 = grande machine ( 1 clé a molette utilisée ), 5 = coffre ouvert, 6 = machine réparée, 7 = grande machine réparée
    int inv; // la valeur de l'objet contenu dans le coffre ( 0 si machine )
} special;

typedef struct tile {
    int contenu; // -5 = vide, -2 = mur, -1 = porte, 0 = sol, 1 = personnage, 2 = monstre, 3 = coffre / machine
    monstre mstr;
    special spe; // .type signifie le cote si porte (0 = haut, 1 = gauche, 2 = bas, 3 = droite)
} tile;

typedef struct salle {
    int num;
    int largeur;
    int longueur;
    int posX;
    int posY;
    tile ** cases; 
} salle ;

extern int musique(int num_musique);
extern int creeMap(void);
extern int jeu(void);
extern void* play_music(void* arg);
extern int generation(int longueur, int largeur, int num_salle, int cote);

extern tile contenuCase;
extern int Xcamera;
extern int Ycamera;
extern tile **map;

#endif