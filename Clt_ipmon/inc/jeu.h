#ifndef JEU_H
#define JEU_H

#include "main.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <time.h>

typedef struct{
	int x;
	int y;
}Coord;

typedef struct Dresseur_aff{
	SDL_Rect perso, positiontexte;
	SDL_Surface* texte;
	struct Dresseur_aff* next;
}Dresseur_aff;




Dresseur_aff* ajouter_dresseur_aff(Dresseur_aff *dresseur_list_jeu, SDL_Rect *perso, SDL_Rect *positionTexte, SDL_Surface *texte);
Dresseur_aff* vider_dresseur_liste(Dresseur_aff *dresseur_list_jeu);
Dresseur_aff* jeuAfficherDresseurs(int sock,char* map, TTF_Font *police);
void jeuDeplacement(SDL_Rect perso, Coord *persoAvant, int sock);
int jeuCombat(int sock);
int affichage_combat(int msg, int sock);
#endif
