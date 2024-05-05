#include <stdio.h>
#include <stdlib.h>

/*
	Windows : gcc src/generation.c -o bin\progGeneration
	Windows sans terminal qui ouvre : gcc src/generation.c -o bin\progGeneration -mwindows
	Linux : gcc generation.c -o progGeneration

	Flags render
	SDL_RENDERER_SOFTWARE
	SDL_RENDERER_ACCELERATED
	SDL_RENDERER_PRESENTVSYNC
	SDL_RENDERER_TARGETTEXTURE
*/

//message erreur
void erreur(char message){
	printf("ERREUR : %s\n",message);
	exit(0);
}

typedef struct {
    int largeur;
    int longueur;
    int posX;
    int posY;
    int ** cases;
} salle ;

salle generation (int graine, salle vaisseau, int numSalle) {
    
    
	return vaisseau;
}

