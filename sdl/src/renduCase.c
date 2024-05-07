#include <SDL.h>
#include <SDL_mixer.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <time.h>

#include "texture.h"



/*
	Windows : gcc src\*.c -o bin\progMain.exe -I include -L lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_mixer -mwindows
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

extern personnage perso;

void SDL_ExitWithError(const char *message);

void SDL_LimitFPS(unsigned int limit);

int camera(int argc, char **argv);

extern int mouvementHaut(void);
extern int mouvementGauche(void);
extern int mouvementBas(void);
extern int mouvementDroite(void);

extern int texture( int argc, char **argv);
extern int creeMap(void);
extern int actualiserMap(void);

int main (int argc, char **argv) {

    int caseLongueur = (WINDOW_WIDTH / 100);
    int caseLargeur = (WINDOW_HEIGHT / 100);
	srand( time( NULL ) );

	//initialisation video et audio
	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0){
		SDL_ExitWithError("Initialisation SDL");

	}

	/* PROBLEME COMPILATION AUDIO !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! /
	if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) != 0){
		SDL_ExitWithError("Impossible de charger SDL mixer");

	}
	/* PROBLEME COMPILATION AUDIO !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

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

    //creation musique

	/* PROBLEME COMPILATION AUDIO !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! /
	Mix_Music * musique = NULL;
	musique = Mix_LoadMUS("src/musique/vaisseau.mp3");
	if(musique == NULL){
		Mix_FreeMusic(musique);
		SDL_ExitWithError("Impossible de charger la musique");
	}
	/* PROBLEME COMPILATION AUDIO !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

    // Création cases pour les textures

    
	extern int Xcamera;
	extern int Ycamera;
	extern int **map;
	
    if (creeMap() != 0){
		SDL_ExitWithError("Impossible de cree la map");
	}

	if (camera(argc, argv) != EXIT_SUCCESS){
		SDL_ExitWithError("Probleme fonction camera");
	}
	
	// Boucle de jeu ******************************************************************************** /

	/* PROBLEME COMPILATION AUDIO !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! /
	if(Mix_PlayMusic(musique, -1) != 0){
		SDL_ExitWithError("Impossible de jouer la musique");
	}
	/* PROBLEME COMPILATION AUDIO !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

	SDL_RenderPresent(renderer);
	printf("fini");
	SDL_bool continuer = SDL_TRUE;
	unsigned int frame_limit = 0;

	frame_limit = SDL_GetTicks() + FPS_LIMIT;
	SDL_LimitFPS(frame_limit);
	frame_limit = SDL_GetTicks() + FPS_LIMIT;

	//gestion des évènements
	while(continuer){
		SDL_Event event;

		while(SDL_PollEvent(&event)){

			frame_limit = SDL_GetTicks() + FPS_LIMIT;
			camera(argc, argv);
			SDL_RenderPresent(renderer);
			SDL_LimitFPS(frame_limit);
			frame_limit = SDL_GetTicks() + FPS_LIMIT;

			switch (event.type){

				case SDL_KEYDOWN:

					switch (event.key.keysym.sym) {
						case SDLK_z:
						case SDLK_UP:
							if(mouvementHaut() != EXIT_SUCCESS){
								SDL_ExitWithError("Erreur mouvement haut");
							}
							continue;

						case SDLK_q:
						case SDLK_LEFT:
							if(mouvementGauche() != EXIT_SUCCESS){
								SDL_ExitWithError("Erreur mouvement gauche");
							}
							continue;

						case SDLK_s:
						case SDLK_DOWN:
							if(mouvementBas() != EXIT_SUCCESS){
								SDL_ExitWithError("Erreur mouvement bas");
							}
							continue;

						case SDLK_d:
						case SDLK_RIGHT:
							if(mouvementDroite() != EXIT_SUCCESS){
								SDL_ExitWithError("Erreur mouvement droite");
							}
							continue;

						case SDLK_SPACE:
							if (creeMap() == 0){
								printf("\ngud\n\n");
							}
							else {
								printf("\nuh oh\n\n");
							}
							continue;

						case SDLK_TAB:
							continuer = SDL_FALSE;
							break;

						default:
							continue;
					}

				//quitter le programme
				case SDL_QUIT:
					continuer = SDL_FALSE;
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

	// fin programme / libération mémoire ************************************************************************** /

	/* PROBLEME COMPILATION AUDIO !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! /
	Mix_FreeMusic(musique);
	/* PROBLEME COMPILATION AUDIO !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	/* PROBLEME COMPILATION AUDIO !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! /
	Mix_CloseAudio();
	/* PROBLEME COMPILATION AUDIO !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

	SDL_Quit();
	return EXIT_SUCCESS;
}

// afficher les images de la camera
int camera(int argc, char ** argv){

	int Xcase,Ycase;

	for ( Xcase = 1 ; Xcase < ( (WINDOW_WIDTH / 100) - 1) ; Xcase ++ ) {
		
        for ( Ycase = 1 ; Ycase < ( (WINDOW_HEIGHT / 100) - 1) ; Ycase ++ ) {

			printf("%d\n",(Xcamera + Xcase));
			contenuCase = map[Xcamera + Xcase][Ycamera + Ycase];
			//contenuCase = -5;
			//printf("%d",contenuCase);
            Case.x = Xcase * 100;
            Case.y = Ycase * 100;
            Case.w = WINDOW_WIDTH / (WINDOW_WIDTH / 100) ;
	        Case.h = WINDOW_HEIGHT / (WINDOW_HEIGHT / 100) ;
            //printf("%d et %d et %d et %d\n",Case.x,Case.y,Case.w,Case.h);
            if(SDL_RenderDrawRect(renderer, &Case) != 0){
		        SDL_ExitWithError("Impossible de dessiner une case");
	        }
			if(texture(argc, argv ) != EXIT_SUCCESS){
				SDL_ExitWithError("Fonction texture interompue");
			}
        }
    }
	perso.frameAnimation ++;
	SDL_RenderPresent(renderer);
	return EXIT_SUCCESS;


}

//message erreur
void SDL_ExitWithError(const char *message){
	SDL_Log("ERREUR : %s > %s\n",message, SDL_GetError());
	
	/* PROBLEME COMPILATION AUDIO !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! /
	Mix_CloseAudio();
	/* PROBLEME COMPILATION AUDIO !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

	SDL_Quit();
	exit(EXIT_FAILURE);
}

// limite fps
void SDL_LimitFPS(unsigned int limit){
	unsigned int ticks = SDL_GetTicks();

	if (limit< ticks) {
		return;
	}
	else if (limit > ticks + FPS_LIMIT){
		SDL_Delay(FPS_LIMIT);
	}
	else {
		SDL_Delay(limit - ticks);
	}



}

