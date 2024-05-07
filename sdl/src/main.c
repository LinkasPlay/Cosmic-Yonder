#include <SDL.h>
#include <SDL_mixer.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <time.h>

#include "texture.h"

extern int jeu(int argc, char **argv);

void clean_ressources(SDL_Window *w, SDL_Renderer *r, SDL_Texture *t){
    
    if(t != NULL){
        SDL_DestroyTexture(t);
    }
    if(r != NULL){
        SDL_DestroyRenderer(r);
    }
    if(w != NULL){
        SDL_DestroyWindow(w);
    }
}

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

int main(int argc, char **argv){

    SDL_Surface *picture = NULL;
    SDL_Texture *texture = NULL;
    SDL_Rect dest_rect = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};

    if(SDL_Init(SDL_INIT_VIDEO) != 0){
        SDL_Log("Erreur : Initialisation SDL > %s\n", SDL_GetError());
        clean_ressources(NULL,NULL,NULL);
        exit(EXIT_FAILURE);
    }

    window = SDL_CreateWindow("Cosmic Yonder", 
                                SDL_WINDOWPOS_CENTERED,(SDL_WINDOWPOS_CENTERED),WINDOW_WIDTH,WINDOW_HEIGHT, 
                                0);

    if(window == NULL){
        SDL_Log("Erreur : Creation fenetre echouee > %s\n", SDL_GetError());
        clean_ressources(NULL,NULL,NULL);
        exit(EXIT_FAILURE);
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    if(renderer == NULL){
        SDL_Log("Erreur : Creation fenetre echouee > %s\n", SDL_GetError());
        clean_ressources(window,NULL,NULL);
        exit(EXIT_FAILURE);
    }

    picture = SDL_LoadBMP("src/image/spacet.bmp");
    if(picture == NULL){
        SDL_Log("Erreur : Creation fenetre echouee > %s\n", SDL_GetError());
        clean_ressources(window,renderer,NULL);
        exit(EXIT_FAILURE);
    }

    texture = SDL_CreateTextureFromSurface(renderer, picture);
    SDL_FreeSurface(picture);
    if(texture == NULL){
        SDL_Log("Erreur : Creation fenetre echouee > %s\n", SDL_GetError());
        clean_ressources(window,renderer,NULL);
        exit(EXIT_FAILURE);
    }

    if ((SDL_QueryTexture(texture, NULL, NULL, &dest_rect.w, &dest_rect.h)) != 0){
        SDL_Log("Erreur : Creation fenetre echouee > %s\n", SDL_GetError());
        clean_ressources(window,renderer,texture);
        exit(EXIT_FAILURE);
    }

    if ((SDL_RenderCopy(renderer, texture, NULL, &dest_rect)) != 0){
        SDL_Log("Erreur : Creation fenetre echouee > %s\n", SDL_GetError());
        clean_ressources(window,renderer,texture);
        exit(EXIT_FAILURE);
    }

    SDL_RenderPresent(renderer);
    SDL_Delay(5000);

    if (jeu (argc, argv) != EXIT_SUCCESS) {
        SDL_Log("Erreur : jeu a planté > %s\n", SDL_GetError());
        clean_ressources(NULL,NULL,NULL);
        exit(EXIT_FAILURE);
    }

    clean_ressources(window, renderer, texture);


    return EXIT_SUCCESS;
}