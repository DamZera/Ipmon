#ifndef JEU_H
#define JEU_H

#include "main.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <time.h>
#include <stdbool.h> 

typedef struct{
    int x;
    int y;
}Coord;

typedef struct Dresseur_aff Dresseur_aff;

struct Dresseur_aff {
    SDL_Rect perso, positiontexte;
    SDL_Surface* texte;
    Dresseur_aff* next;
};

typedef struct {
    struct sockaddr_in* srvaddr;
    int socket;
    Coord* xyPlayer;
    Dresseur* player;
    Dresseur_aff* listOfPlayer;
    bool stopThread;
}ClientContext;

Dresseur_aff* ajouter_dresseur_aff(Dresseur_aff *dresseur_list_jeu, SDL_Rect *perso,
    SDL_Rect *positionTexte, SDL_Surface *texte);
Dresseur_aff* vider_dresseur_liste(Dresseur_aff *dresseur_list_jeu);
void setListOfPlayers(TTF_Font *police, char* buffer, ClientContext* cltCtx);
void jeuDeplacement(SDL_Rect perso, Coord *persoAvant);
int jeuCombat(int sock);
int affichage_combat(int msg, int sock);

Ipmon* ajouterIpmon(Ipmon* ipmon_list,
    int id,
    char* nom,
    char* etat,
    char* type,
    int typeEntier,
    int pv,
    int agilite,
    int niveau,
    int puissance_attaque,
    char* nom_attaque,
    int precision_attaque,
    int puissance_defense,
    int esquive,
    int precision,
    int puissance_attaque_special,
    int precision_attaque_special,
    char* nom_attaque_special,
    int puissance_defense_special);

void* threadUpdatePositionAndListOfPlayer(void *data);

#endif
