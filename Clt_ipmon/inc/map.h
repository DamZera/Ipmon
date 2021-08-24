#include <SDL2/SDL.h>

#include "logger.h"

typedef struct // tuiles (=images)
{
    SDL_Rect R;
    int collision;
    int changerMap;
    int combat;
} TuilePropriete;

typedef struct // Tileset
{
    int LARGEUR_TILE,HAUTEUR_TILE;
    int nbtilesX,nbtilesY;
    SDL_Texture* tileset;
    TuilePropriete* props;
    Uint16** schema; // permet 
    int nbtiles_largeur_monde,nbtiles_hauteur_monde;
    int xscroll,yscroll;
    int largeur_fenetre,hauteur_fenetre;
} Map;

void logSDLError(const char* msg);
SDL_Texture* loadTexture(const char* fichier_image, SDL_Renderer *ren);
Map* ChargerMap(SDL_Renderer *renderer, const char* fic,int largeur_fenetre,int hauteur_fenetre);
int AfficherMap(Map* m, SDL_Renderer* renderer,int xscroll,int yscroll);
int LibererMap(Map* m);
int CollisionDecor(Map* carte,SDL_Rect* perso,SDL_Surface* screen);
int FocusScrollCenter(Map* carte,SDL_Rect* perso);

