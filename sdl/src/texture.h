#ifndef TEXTURE_H
#define TEXTURE_H

#define DIMENSION_MAP 300
#define WINDOW_WIDTH 1500
#define WINDOW_HEIGHT 900
#define FPS_LIMIT 16

extern void SDL_ExitWithError(const char *message);
extern int texture( int argc, char **argv);
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
    int frameAnimation;
} personnage;

typedef struct salle {
    int largeur;
    int longueur;
    int posX;
    int posY;
    int ** cases;
} salle ;

extern int creeTab(void);
extern int contenuCase;
extern SDL_Window *window;
extern SDL_Renderer *renderer;
extern int Xcamera;
extern int Ycamera;
extern int **map;

#endif