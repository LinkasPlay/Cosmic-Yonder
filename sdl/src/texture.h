#ifndef TEXTURE_H
#define TEXTURE_H

#define DIMENSION_MAP 300

extern void SDL_ExitWithError(const char *message);
extern int texture( int argc, char **argv);
/*
int contenuCase = 0;
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
SDL_Rect Case;
*/
extern int creeTab(void);
extern int contenuCase;
extern SDL_Window *window;
extern SDL_Renderer *renderer;
extern int Xcamera;
extern int Ycamera;
extern int **map;

#endif