#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "texture.h"

#define WINDOW_WIDTH 1600
#define WINDOW_HEIGHT 1100

/*
	Windows : gcc src/renduCase.c -o bin/progRenduCase -I include -L lib -lmingw32 -lSDL2main -lSDL2
	Windows sans terminal qui ouvre : gcc src/renduCase.c -o bin/progRenduCase -I include -L lib -lmingw32 -lSDL2main -lSDL2 -mwindows
	Linux : gcc renduCase.c $(sdl2-config __cflags --libs) -o progRenduCase

	Flags render
	SDL_RENDERER_SOFTWARE
	SDL_RENDERER_ACCELERATED
	SDL_RENDERER_PRESENTVSYNC
	SDL_RENDERER_TARGETTEXTURE
*/

int contenuCase = 0;
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
SDL_Rect Case;

void SDL_ExitWithError(const char *message);
extern int texture( int argc, char **argv);
extern int creeTab(void);

int main (int argc, char **argv) {
   	/*/ 
	int contenuCase = 0;
	SDL_Window *window = NULL;
	SDL_Renderer *renderer = NULL;
	//
	extern int contenuCase;
	extern SDL_Window *window;
	extern SDL_Renderer *renderer;
	/*/
    int caseLongueur = (WINDOW_WIDTH / 100);
    int caseLargeur = (WINDOW_HEIGHT / 100);
	srand( time( NULL ) );
	//initialisation video et audio
	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0){
		SDL_ExitWithError("Initialisation SDL");

	}
	
	//creation fenètre (nom fenétre, position x fenètre, position y, largeur, hauteur, drapeaux)
	window = SDL_CreateWindow("Cosmic Yonder",
								SDL_WINDOWPOS_CENTERED,(SDL_WINDOWPOS_CENTERED),WINDOW_WIDTH,WINDOW_HEIGHT,
								0);
	if(window == NULL){
		SDL_ExitWithError("Creation fenetre echouee");
	}

	//creation rendu
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
	if(renderer == NULL){
		SDL_ExitWithError("Creation rendu echouee");
	}

    //couleur fond
	if(SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE) != 0 ){
		SDL_ExitWithError("Impossible de changer la couleur pour le rendu");
	}

	SDL_Rect rectangle;
	rectangle.x = WINDOW_WIDTH / caseLongueur;
	rectangle.y = WINDOW_HEIGHT / caseLargeur;
	rectangle.w = WINDOW_WIDTH - 2 * ( WINDOW_WIDTH / caseLongueur );
	rectangle.h = WINDOW_HEIGHT - 2 * ( WINDOW_HEIGHT / caseLargeur );

	if(SDL_RenderFillRect(renderer, &rectangle) != 0){
		SDL_ExitWithError("Impossible de dessiner le fond");
	}

	SDL_RenderPresent(renderer);
	SDL_Delay(100);

    //couleur cases
	if(SDL_SetRenderDrawColor(renderer, 232, 31, 31, SDL_ALPHA_OPAQUE) != 0 ){
		SDL_ExitWithError("Impossible de changer la couleur pour le rendu");
	}

	/*effacement rendu
	if(SDL_RenderClear(renderer) != 0){
		SDL_ExitWithError("Efffacement rendu echouee");
	}
    */

    //extern SDL_Rect Case;
	

    // Création cases pour les textures
    int Xcase,Ycase;
	//for ( Xcase = ( WINDOW_WIDTH / caseLongueur ) ; Xcase < WINDOW_WIDTH - ( WINDOW_WIDTH / caseLongueur ) ; Xcase + ( WINDOW_WIDTH / caseLongueur ) ) {
    //    for ( Ycase = ( WINDOW_HEIGHT / caseLargeur ) ; Ycase < WINDOW_HEIGHT - ( WINDOW_HEIGHT / caseLargeur ) ; Ycase + ( WINDOW_HEIGHT / caseLargeur ) ) {

    for ( Xcase = 1 ; Xcase < (caseLongueur - 1) ; Xcase ++ ) {
        for ( Ycase = 1 ; Ycase < (caseLargeur - 1) ; Ycase ++ ) {

			// case avec mur
			if ( (Xcase == 1) || (Ycase == 1) || (Xcase == (caseLongueur - 2)) || (Ycase == (caseLargeur - 2)) ) {
				contenuCase = -2;
				// case avec porte
				if ( ( (Xcase == (caseLongueur / 2) ) && ( Ycase == ( 1 || (caseLargeur - 2) ) ) ) || ( (Ycase == (caseLargeur / 2) ) && ( Xcase == ( 1 || (caseLongueur - 2) ) ) ) ){
					if ( rand()%3 != 0){
						contenuCase = -1;
					}
				}
			}
			// case avec monstre
			else if ( rand()%10 == 0){
				contenuCase = 2;
			}
			// case avec machine
			else if ( rand()%15 == 0){
				contenuCase = 3;
			}
			//case vide
			else{
				contenuCase = 0;
			}
			if ( (Xcase == (caseLongueur / 2) ) && (Ycase == (caseLargeur / 2) ) ){
				contenuCase = 1;
			}
			
            Case.x = Xcase * 100;
            Case.y = Ycase * 100;
            Case.w = WINDOW_WIDTH / caseLongueur ;
	        Case.h = WINDOW_HEIGHT / caseLargeur ;
            //printf("%d et %d et %d et %d\n",Case.x,Case.y,Case.w,Case.h);
            if(SDL_RenderDrawRect(renderer, &Case) != 0){
		        SDL_ExitWithError("Impossible de dessiner une case");
	        }
			if(texture(argc, argv ) != EXIT_SUCCESS){
				SDL_ExitWithError("Fonction texture interompue");
			}
        }
    }
	
	SDL_RenderPresent(renderer);
	printf("fini");
	SDL_bool program_launched = SDL_TRUE;

	//gestion des évènements
	while(program_launched){
		SDL_Event event;

		while(SDL_PollEvent(&event)){

			switch (event.type){

				case SDL_KEYDOWN:

					switch (event.key.keysym.sym) {
						case SDLK_z:
						case SDLK_UP:
							printf("z ou flèche haut\n");
							continue;

						case SDLK_q:
						case SDLK_LEFT:
							printf("q ou flèche gauche\n");
							continue;

						case SDLK_s:
						case SDLK_DOWN:
							printf("s ou flèche bas\n");
							continue;

						case SDLK_d:
						case SDLK_RIGHT:
							printf("d ou flèche droite\n");
							continue;

						case SDLK_SPACE:
							if (creeTab() == 0){
								printf("\ngud\n\n");
							}
							else {
								printf("\nuh oh\n\n");
							}
							continue;

						case SDLK_TAB:
							program_launched = SDL_FALSE;
							break;

						default:
							continue;
					}



				//quitter le programme
				case SDL_QUIT:
					program_launched = SDL_FALSE;
					break;
		
				default:
					break;
			}
		}

	}

    //effacement rendu
	if(SDL_RenderClear(renderer) != 0){
		SDL_ExitWithError("Efffacement rendu echouee");
	}

	// fin programme / libération mémoire
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return EXIT_SUCCESS;
}

//message erreur
void SDL_ExitWithError(const char *message){
	SDL_Log("ERREUR : %s > %s\n",message, SDL_GetError());
	SDL_Quit();
	exit(EXIT_FAILURE);
}
