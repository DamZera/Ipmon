#include <SDL/SDL.h>


typedef struct // propriété des tuiles (=images)
{
	SDL_Rect R;
	int collision;
	int changerMap;
	int combat;
} TuilePropriete;

typedef struct // ma structure map au cas où j'utilise d'autres tileset (j'envisage!!!)
{
	int LARGEUR_TILE,HAUTEUR_TILE;
	int nbtilesX,nbtilesY;
	SDL_Surface* tileset;
	TuilePropriete* props;
	Uint16** schema; // permet 
	int nbtiles_largeur_monde,nbtiles_hauteur_monde;
	int xscroll,yscroll;
	int largeur_fenetre,hauteur_fenetre;
} Map;

Map* ChargerMap(const char* fic,int largeur_fenetre,int hauteur_fenetre);
int AfficherMap(Map* m,SDL_Surface* screen,int xscroll,int yscroll);
int LibererMap(Map* m);
int CollisionDecor(Map* carte,SDL_Rect* perso,SDL_Surface* screen);
int FocusScrollCenter(Map* carte,SDL_Rect* perso);

