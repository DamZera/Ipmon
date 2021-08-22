#include <string.h>

#include <SDL2/SDL_image.h>

#include "map.h"

#define CACHE_SIZE 5000
#define LARGEUR_FENETRE 800
#define HAUTEUR_FENETRE 800

#define TILE_WIDTH  25
#define TILE_HEIGHT 25

void logSDLError(const char* msg) {
        printf("%s error : %s", msg, SDL_GetError());
}

SDL_Texture* loadTexture(const char* fichier_image, SDL_Renderer *ren)
{
	SDL_Texture *texture = NULL;
	SDL_Surface* imgSurface = SDL_LoadBMP(fichier_image);
	
	if (imgSurface == NULL)
	{
		logSDLError("LoadBMP");
	}
	else
	{
		texture = SDL_CreateTextureFromSurface(ren, imgSurface);
        SDL_FreeSurface(imgSurface);

		if (texture == NULL) {
        	logSDLError("CreateTextureFromSurface");
        }
	}

	return texture;
}

void loadTilesetInMap(SDL_Renderer *pRenderer, FILE* F,Map* m) // chargement du tileset (partie tileset fichier texte)
{
	int numtile,i,j;
	char buf[CACHE_SIZE];  // un buffer, petite m�moire cache
	char buf2[CACHE_SIZE];  // un buffer, petite m�moire cache
	fscanf(F,"%s",buf); // #tileset
	fscanf(F,"%s",buf); // nom du fichier
	printf("mon buf is %s\n", buf);
	m->tileset = loadTexture(buf, pRenderer);
	fscanf(F,"%d %d",&m->nbtilesX,&m->nbtilesY);
	//int w, h;
	//SDL_QueryTexture(m->tileset, NULL, NULL, &w, &h);
	m->LARGEUR_TILE = 25;//m->tileset->w/m->nbtilesX;
	m->HAUTEUR_TILE = 25;//m->tileset->h/m->nbtilesY;
	m->props = malloc(m->nbtilesX*m->nbtilesY*sizeof(TuilePropriete));
	for(j=0,numtile=0;j<m->nbtilesY;j++)
	{
		for(i=0;i<m->nbtilesX;i++,numtile++)
		{
			m->props[numtile].R.w = m->LARGEUR_TILE;
			m->props[numtile].R.h = m->HAUTEUR_TILE;
			m->props[numtile].R.x = i*m->LARGEUR_TILE;
			m->props[numtile].R.y = j*m->HAUTEUR_TILE;
			fscanf(F,"%s %s",buf,buf2);
			m->props[numtile].collision = 0;
			m->props[numtile].changerMap = 0;
			m->props[numtile].combat = 0;
			if (strcmp(buf2,"collision")==0){
				m->props[numtile].collision = 1;
			}
			else if(strcmp(buf2,"herbe")==0){
				m->props[numtile].combat = 1;
			}
			else if(strcmp(buf2, "changerMap") == 0){
				m->props[numtile].changerMap = 2;
			}
			
		}
	}
}

void loadLevelInMap(FILE* F,Map* m) // charger la partie #level dans un fichier texte
{
	int i,j;
	char buf[CACHE_SIZE];  
	fscanf(F,"%s",buf); // #level dans un fichier texte
	fscanf(F,"%d %d",&m->nbtiles_largeur_monde,&m->nbtiles_hauteur_monde);
	m->schema = malloc(m->nbtiles_largeur_monde*sizeof(Uint16*));
	for(i=0;i<m->nbtiles_largeur_monde;i++)
		m->schema[i] = malloc(m->nbtiles_hauteur_monde*sizeof(Uint16));
	for(j=0;j<m->nbtiles_hauteur_monde;j++)
	{
		for(i=0;i<m->nbtiles_largeur_monde;i++)
		{
			int tmp;
			fscanf(F,"%d",&tmp);
			if (tmp>=m->nbtilesX*m->nbtilesY)
			{
				printf("map tuile hors limite\n");
				SDL_Quit();
				system("pause");
				exit(-1);
			}
			m->schema[i][j] = tmp;
		}
	}
}

Map* ChargerMap(SDL_Renderer *pRenderer, const char* level, int largeur_fenetre, int hauteur_fenetre) // charger le fichier texte entier
{
	FILE* file;
	Map* m;
	file = fopen(level,"r");
	if (!file)
	{
		printf("file %s not found !! \n",level);
		SDL_Quit();
		exit(-1);
	}
	m = malloc(sizeof(Map));
	printf("\nbefore charger tileset\n");
	loadTilesetInMap(pRenderer, file, m);
	printf("\ncharger tileset\n");
	loadLevelInMap(file,m);
	printf("\ncharger level\n");
	m->largeur_fenetre = largeur_fenetre;
	m->hauteur_fenetre = hauteur_fenetre;
	m->xscroll = 0;
	m->yscroll = 0;
	fclose(file);
	return m;
}

int AfficherMap(Map* m, SDL_Renderer* renderer, int xscroll,int yscroll) // Affiche la map � l'�cran
{
	printf("AfficherMap :: START\n");
	int i,j;
	SDL_Rect Rect_dest;
	Rect_dest.w = TILE_WIDTH;
	Rect_dest.h = TILE_HEIGHT;
	
	SDL_Rect Rect_src;
	Rect_src.y = 0;
	Rect_src.w = TILE_WIDTH;
	Rect_src.h = TILE_HEIGHT;	

	int numero_tile;
	int minx,maxx,miny,maxy;
	minx = m->xscroll / m->LARGEUR_TILE;
	miny = m->yscroll / m->HAUTEUR_TILE;
	maxx = (m->xscroll + m->largeur_fenetre)/m->LARGEUR_TILE;
	maxy = (m->yscroll + m->hauteur_fenetre)/m->HAUTEUR_TILE;
	for(i=minx;i<=maxx;i++)
	{
		for(j=miny;j<=maxy;j++)
		{
			Rect_dest.x = i*m->LARGEUR_TILE - m->xscroll;
			Rect_dest.y = j*m->HAUTEUR_TILE - m->yscroll;
			
			if (i<0 || i>=m->nbtiles_largeur_monde || j<0 || j>=m->nbtiles_hauteur_monde){
				numero_tile = 0;
			
			}
			else{
				numero_tile = m->schema[i][j];
				Rect_src.x = m->props[numero_tile].R.x;
				Rect_src.y = m->props[numero_tile].R.y;
				//SDL_UpdateTexture(sdlTexture, NULL, m->tileset->pixels, m->tileset->pitch);
				SDL_RenderCopy(renderer, m->tileset, &Rect_src, &Rect_dest);
			}
		}
	}

	printf("AfficherMap :: END\n");

	return 0;
}

int LibererMap(Map* m)
{
	// TODO to refactor pls
	SDL_FreeSurface(m->tileset);
	free(m->schema);
	free(m->props);
	//free(m);
	return 0;
}

int CollisionDecor(Map* carte, SDL_Rect* perso, SDL_Surface* screen){
	
	//Map* carte1;
	//Map* carte2;
	
	//carte1 = ChargerMap("level.txt",LARGEUR_FENETRE,HAUTEUR_FENETRE);
	//carte2 = ChargerMap("tilesetIPMON.txt",LARGEUR_FENETRE,HAUTEUR_FENETRE);

	int xmin,xmax,ymin,ymax,i,j,indicetile,xminbord,yminbord;
	xmin = perso->x / carte->LARGEUR_TILE;
	ymin = perso->y / carte->HAUTEUR_TILE;
	xminbord = (perso->x - perso->w -1) / carte->LARGEUR_TILE;
	yminbord = (perso->y - perso->h -1) / carte->HAUTEUR_TILE;
	xmax = (perso->x + perso->w -1) / carte->LARGEUR_TILE;
	ymax = (perso->y + perso->h -1) / carte->HAUTEUR_TILE;
	if (xminbord<0 || yminbord<0 || xmax>=carte->nbtiles_largeur_monde || ymax>=carte->nbtiles_hauteur_monde)
		return 1;
	for(i=xmin;i<=xmax;i++)
	{
		for(j=ymin;j<=ymax;j++)
		{
			indicetile = carte->schema[i][j];
			if (carte->props[indicetile].collision){
				return 1;
			}
			else if(carte->props[indicetile].combat){
				return 2;
			}
			// else if(carte->props[indicetile].changerMap){
			// 	LibererMap(carte);

			// 	carte1 = ChargerMap("level.txt",LARGEUR_FENETRE,HAUTEUR_FENETRE);
			// 	perso->x = 270;
			// 	perso->y = 500;
			// 	perso->w = 25;
			// 	perso->h = 25;
				
			// 	AfficherMap(carte1,screen,carte1->xscroll,carte1->yscroll);
				
			// 	return 0;
			// }
			
			

			// else if(carte1->props[indicetile].changerMap == 2){

			// 	LibererMap(carte1);
				
				
			// 	perso->x =264;
			// 	perso->y =550;
			// 	perso->w = 25;
			// 	perso->h = 25;
				
			// 	AfficherMap(carte2,screen,carte2->xscroll,carte2->yscroll);
				
			// 	printf("Changement map");
			// 	return 0;
			// }

		}
	}
	return 0;
}

void ClampScroll(Map* m) {// permettre le scrolling
	if (m->xscroll<0)
		m->xscroll=0;
	if (m->yscroll<0)
		m->yscroll=0;
	if (m->xscroll>m->nbtiles_largeur_monde*m->LARGEUR_TILE-m->largeur_fenetre-1)
		m->xscroll=m->nbtiles_largeur_monde*m->LARGEUR_TILE-m->largeur_fenetre-1;
	if (m->yscroll>m->nbtiles_hauteur_monde*m->HAUTEUR_TILE-m->hauteur_fenetre-1)
		m->yscroll=m->nbtiles_hauteur_monde*m->HAUTEUR_TILE-m->hauteur_fenetre-1;
}

int FocusScrollCenter(Map* carte,SDL_Rect* perso) // focus scrolling sur le dresseur
{
	printf("FocusScrollCenter :: START\n");
	carte->xscroll = perso->x + perso->w/2 - carte->largeur_fenetre/2;
	carte->yscroll = perso->y + perso->h/2 - carte->hauteur_fenetre/2;
	ClampScroll(carte);
	printf("FocusScrollCenter :: END\n");
	return 0;
}


 /*void InitialiserMap(Map* map[]){
	int i;
		for(i=0; i<1;i++){
			map[0].LARGEUR_TILE = 25;
			map[0].HAUTEUR_TILE=25;
			map[0].*/