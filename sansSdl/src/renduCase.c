#include <stdlib.h>
#include <unistd.h>

#include <ncurses.h>

#include "texture.h"



/*
	Windows : gcc src\*.c -o bin\progMain.exe -I include -L lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_mixer -mwindows
	Windows : gcc sansSdl\*.c -o bin\progMain.exe -I include -L lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_mixer -mwindows
	Linux : gcc renduCase.c $(sdl2-config __cflags --libs) -o progRenduCase

	Flags render
	SDL_RENDERER_SOFTWARE
	SDL_RENDERER_ACCELERATED
	SDL_RENDERER_PRESENTVSYNC
	SDL_RENDERER_TARGETTEXTURE
*/

tile contenuCase;

extern personnage perso;

int camera(void);

extern int mouvementHaut(void);
extern int mouvementGauche(void);
extern int mouvementBas(void);
extern int mouvementDroite(void);
extern int attaqueEpee(void);

extern int texture(void);
extern int creeMap(void);
extern int actualiserMap(void);

int jeu (void) {

    int caseLongueur = (WINDOW_WIDTH / 100);
    int caseLargeur = (WINDOW_HEIGHT / 100);
  
	extern int Xcamera;
	extern int Ycamera;
	extern tile **map;
	
	if (perso.lvl == 0){
		if (creeMap() != 0){
			printf("Impossible de cree la map");
		}
	}

	if (camera() != EXIT_SUCCESS){
		printf("Probleme fonction camera");
	}
	
	// Boucle de jeu ******************************************************************************** /

	printf("fini");
	unsigned int frame_limit = 0;

	//gestion des évènements

	/*
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

						case SDLK_e: 
						case SDLK_SPACE:
							if(attaqueEpee() != EXIT_SUCCESS){
								SDL_ExitWithError("Erreur attaque");
							}
							continue;

						case SDLK_z: // mouvement vers le haut
						case SDLK_UP:
							if(mouvementHaut() != EXIT_SUCCESS){
								SDL_ExitWithError("Erreur mouvement haut");
							}
							continue;

						case SDLK_q: // mouvement vers la gauche
						case SDLK_LEFT:
							if(mouvementGauche() != EXIT_SUCCESS){
								SDL_ExitWithError("Erreur mouvement gauche");
							}
							continue;

						case SDLK_s: // mouvement vers le bas
						case SDLK_DOWN:
							if(mouvementBas() != EXIT_SUCCESS){
								SDL_ExitWithError("Erreur mouvement bas");
							}
							continue;

						case SDLK_d: // mouvement vers la droite
						case SDLK_RIGHT:
							if(mouvementDroite() != EXIT_SUCCESS){
								SDL_ExitWithError("Erreur mouvement droite");
							}
							continue;

						case SDLK_TAB: // recharger la map
							if (actualiserMap() == 0){
								printf("\ngud\n\n");
							}
							else {
								printf("\nuh oh\n\n");
							}
							continue;

						//quitter le programme (menu pause dans le futur)
						case SDLK_ESCAPE:
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
	*/

	endwin();

	return EXIT_SUCCESS;
}

// afficher les images de la camera
int camera(void){

	int Xcase,Ycase;

	for ( Xcase = 1 ; Xcase < ( (WINDOW_WIDTH / 100) - 1) ; Xcase ++ ) {
		
        for ( Ycase = 1 ; Ycase < ( (WINDOW_HEIGHT / 100) - 1) ; Ycase ++ ) {

			printf("%d\n",(Xcamera + Xcase));
			contenuCase = map[Xcamera + Xcase][Ycamera + Ycase];
			//contenuCase = -5;
			//printf("%d",contenuCase);
            //Case.x = Xcase * 100;
            //Case.y = Ycase * 100;
            //Case.w = WINDOW_WIDTH / (WINDOW_WIDTH / 100) ;
	        //Case.h = WINDOW_HEIGHT / (WINDOW_HEIGHT / 100) ;
			win = newwin(5, 10, Ycamera, Xcase);
   	 		box(win, 0, 0); // Dessine le cadre de la fenêtre
    		mvwprintw(win, 1, 1, "Chokbar de 2 bz");
    		wrefresh(win);
			if(texture(win) != EXIT_SUCCESS){
				printf("Fonction texture interompue");
			}
			*/
        }
    }
	perso.frameAnimation ++;
	return EXIT_SUCCESS;


}

