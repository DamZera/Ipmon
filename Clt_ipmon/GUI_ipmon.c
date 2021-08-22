#include "GUI_ipmon.h"
#include "jeu.h"

#define SGN(X) (((X)==0)?(0):(((X)<0)?(-1):(1)))
#define ABS2(X) ((((X)<0)?(-(X)):(X)))
// Utilisation de 3 fonctions qui permettent le déplacement
Dresseur_aff *dresseur_list_jeu;
Dresseur *joueur;
SDL_Texture *dresseurHaut = NULL, *dresseurBas = NULL , *dresseurGauche = NULL, *dresseurDroite = NULL, *dresseurActuel = NULL;

void DeplacerVecteur(Uint8* in,int* vx,int* vy,SDL_Rect* perso,SDL_Renderer* pRenderer,int xscroll,int yscroll,SDL_Surface* texte) // Deplacement du dresseur PETIT SOUCIS QUE JE N'ARRIVE PAS A REGLER LORS DE LACHEMENT DE BUTON J'AI TESTE AVEC UNE BOUCLE WHILE MAIS ECRAN NOIR!!!
{	
	printf("DeplacerVecteur :: START\n");
	Dresseur_aff* dresseur = dresseur_list_jeu;

	SDL_Event event;
	SDL_Rect positionsurecran = *perso, positiontexte = *perso;
	SDL_Rect postionpersoB, texteB;
	SDL_Rect imgSize;
	imgSize.x = 0;
	imgSize.y = 0;
	imgSize.w = 20;
	imgSize.h = 30;
	positionsurecran.x -= xscroll;
	positionsurecran.y -= yscroll;
	//positiontexte.x = positionsurecran.x - (texte->w /2) + (perso->w/2);
	//positiontexte.y = positionsurecran.y -10;
	int vitesse = MOVE_SPEED_PLAYER;
	*vx = *vy = 0;

	printf("DeplacerVecteur :: INIT DONE\n");

	//SDL_EnableKeyRepeat(100, 100);
		
	/*while(dresseur != NULL){
		postionpersoB = dresseur->perso;
		texteB = dresseur->positiontexte;
		
		postionpersoB.x -= xscroll;
		postionpersoB.y -= yscroll;
		texteB.x -= xscroll;
		texteB.y -= yscroll;
		
		//SDL_BlitSurface(dresseurBas, NULL, screen, &postionpersoB);
		//SDL_BlitSurface(dresseur->texte, NULL, screen, &texteB);
		dresseur = dresseur->next;
	}*/
	

	if (in[SDL_SCANCODE_UP]){
		*vy = -vitesse;
		dresseurActuel=dresseurHaut;
	}
	else if (in[SDL_SCANCODE_DOWN]){
		*vy = vitesse;
		dresseurActuel=dresseurBas;
	}
	else if (in[SDL_SCANCODE_LEFT]){
		*vx = -vitesse;
		dresseurActuel=dresseurGauche;
	}
	else if (in[SDL_SCANCODE_RIGHT]){
		*vx = vitesse;
		dresseurActuel=dresseurDroite;
	}

	//SDL_EnableKeyRepeat(0, 0);
	//SDL_BlitSurface(*dresseurActuel, NULL, screen, &positionsurecran);
	//SDL_BlitSurface(texte, NULL, screen, &positiontexte);

	SDL_RenderCopy(pRenderer, dresseurActuel, &imgSize, &positionsurecran);
	printf("DeplacerVecteur :: END\n");
}

int EssaiDeplacement(Map* carte,SDL_Rect* perso,int vx,int vy, int sock, SDL_Surface *screen)
{
	SDL_Rect test;
	int result;
	test = *perso;
	test.x+=vx;
	test.y+=vy;
	
	/*Renvoie 1 si collission ou 2 si combat puis enfin 0 pour les autres cas*/
	result = CollisionDecor(carte,&test, screen);
	
	if (result == 0){
		*perso = test;
		return 1;
	}else if(result == 2){
		*perso = test;
		jeuCombat(sock);
		return 1;
	}
	else if(result == 3){
		printf("Apres CollisionDecor");
		
		return 0;
	}
	return 0;
}

void Deplace(Map* carte,SDL_Rect* perso,int vx,int vy,int LARGEUR_TILE,int HAUTEUR_TILE,int sock,SDL_Surface *screen)
{
	printf("Deplace :: START\n");
	int i;
	if (vx>=LARGEUR_TILE || vy>=HAUTEUR_TILE)
	{
		Deplace(carte,perso,vx/2,vy/2,LARGEUR_TILE,HAUTEUR_TILE,sock, screen);
		Deplace(carte,perso,vx-vx/2,vy-vy/2,LARGEUR_TILE,HAUTEUR_TILE,sock, screen);
		return;
	}
	if (EssaiDeplacement(carte,perso,vx,vy,sock, screen)==1)
		return;

	for(i=0;i<ABS2(vx);i++)
	{
		if (EssaiDeplacement(carte,perso,SGN(vx),0,sock, screen)==0)
			break;
	}
	for(i=0;i<ABS2(vy);i++)
	{
		if (EssaiDeplacement(carte,perso,0,SGN(vy),sock, screen)==0)
			break;			
	}
	printf("Deplace :: END\n");
}

void init_dresseur(SDL_Renderer* ren){
	SDL_Surface* dressHaut = SDL_LoadBMP("./images/bmp/dresseurhaut.bmp");
	SDL_Surface* dressBas = SDL_LoadBMP("./images/bmp/dresseurbas.bmp");
	SDL_Surface* dressGauche = SDL_LoadBMP("./images/bmp/dresseurgauche.bmp");
	SDL_Surface* dressDroite = SDL_LoadBMP("./images/bmp/dresseurdroite.bmp");

	// Delete white background and add alpha channel
	SDL_SetColorKey(dressBas, SDL_FALSE, SDL_MapRGB(dressBas->format, 0, 255, 0));
	SDL_SetColorKey(dressHaut, SDL_FALSE, SDL_MapRGB(dressHaut->format, 0, 255, 0));
	SDL_SetColorKey(dressGauche, SDL_FALSE, SDL_MapRGB(dressGauche->format, 0, 255, 0));
	SDL_SetColorKey(dressDroite, SDL_FALSE, SDL_MapRGB(dressDroite->format, 0, 255, 0));

	dresseurHaut = SDL_CreateTextureFromSurface(ren, dressHaut);
	dresseurBas = SDL_CreateTextureFromSurface(ren, dressBas);
	dresseurGauche = SDL_CreateTextureFromSurface(ren, dressGauche);
	dresseurDroite = SDL_CreateTextureFromSurface(ren, dressDroite);

	// Default dresseur texture
	dresseurActuel = dresseurBas;
	 
    SDL_FreeSurface(dressHaut);
	SDL_FreeSurface(dressBas);
	SDL_FreeSurface(dressGauche);
	SDL_FreeSurface(dressDroite);
}

//DEFAULT nom_map "tilesetIPMON.txt" persox "300" persoy "150"
void jeu(int sock, Dresseur *dresseur){
	printf("IN jeu function on map:%s", dresseur->map);
	joueur = dresseur;
	
	struct timeval tv1,tv2;
	long elapsed; // ms

	if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) != 0) {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return 1;
    }

	SDL_Rect perso;
	Coord *persoAvant = malloc(sizeof(Coord));
	SDL_Surface* texte = NULL; 
	//SDL_Surface* dressActuel = SDL_LoadBMP("./images/bmp/dresseurbas.bmp");
	//SDL_SetColorKey(dresseurActuel, SDL_FALSE, SDL_MapRGB(dresseurActuel->format, 0, 255, 0));
	dresseur_list_jeu = NULL;
	
	TTF_Font *police = NULL;
   	SDL_Color couleurNoire = {0, 0, 0}; //couleur noir
	//TTF_Init();
	//police = TTF_OpenFont("./arial.ttf", 12); // police
	//texte = TTF_RenderText_Blended(police, dresseur->pseudo, couleurNoire); // ecriture du texte (exemple)

	
	SDL_Event event;
	Map* carte;
	//carte = malloc(sizeof(Map));
	//Input in;
	int LARGEUR_TILE,HAUTEUR_TILE;
	int vx = 0,vy = 0;
	LARGEUR_TILE = 25;
	HAUTEUR_TILE = 25;
	//memset(&in,0,sizeof(in));

	SDL_Renderer *pRenderer;
	SDL_Window *window = SDL_CreateWindow("My Game Window",
                          SDL_WINDOWPOS_CENTERED,
                          SDL_WINDOWPOS_CENTERED,
                          LARGEUR_FENETRE, HAUTEUR_FENETRE,
                    	  SDL_WINDOW_SHOWN);

	 
    pRenderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    
	if (pRenderer == NULL){
        logSDLError("CreateRenderer");
    }
	//screen = SDL_SetVideoMode(LARGEUR_FENETRE, HAUTEUR_FENETRE, 32, 0);
	SDL_Surface *screen = SDL_CreateRGBSurface(0, LARGEUR_FENETRE, HAUTEUR_FENETRE, 32,
                                        0x00FF0000,
                                        0x0000FF00,
                                        0x000000FF,
                                        0xFF000000);
	SDL_Texture *sdlTexture = SDL_CreateTexture(pRenderer,
                                            SDL_PIXELFORMAT_ARGB8888,
                                            SDL_TEXTUREACCESS_STREAMING,
                                            LARGEUR_FENETRE, HAUTEUR_FENETRE);
	
	carte = ChargerMap(pRenderer, dresseur->map, LARGEUR_FENETRE, HAUTEUR_FENETRE);
	perso.x = dresseur->coodX;
	perso.y = dresseur->coodY;
	persoAvant->x = dresseur->coodX;
	persoAvant->y = dresseur->coodY;
	perso.w = 25;
	perso.h = 25;

	gettimeofday(&tv1,NULL);
	gettimeofday(&tv2,NULL);
	
	init_dresseur(pRenderer);
	
	srand(time(NULL));

	jeuInitjoueur(joueur);

	const Uint8 *state = SDL_GetKeyboardState(NULL);

	while(!state[SDL_SCANCODE_ESCAPE]) // Press ESCAPE to leave
	{	
		// 2 usec
		SDL_RenderClear(pRenderer);

		// Update all Keyboard events at 120 usec
		SDL_PumpEvents();

		jeuDeplacement(perso,persoAvant,sock);

		// 10 usec
		Deplace(carte, &perso, vx, vy, LARGEUR_TILE, HAUTEUR_TILE, sock, screen);
		// 10 usec
		FocusScrollCenter(carte,&perso);

		// 450 usec
		AfficherMap(carte, pRenderer, carte->xscroll, carte->yscroll);

		// 25 usec
		DeplacerVecteur(state, &vx, &vy, &perso, pRenderer, carte->xscroll, carte->yscroll, texte);

		if(elapsed > 250000){
			//printf("UPDATE !\n");
			//dresseur_list_jeu = jeuAfficherDresseurs(sock, dresseur->map, police);
			//gettimeofday(&tv2,NULL);
		}
		
		
		//SDL_UpdateTexture(sdlTexture, NULL, screen->pixels, screen->pitch);

		//SDL_RenderClear(pRenderer);
		//SDL_RenderCopy(pRenderer, sdlTexture, NULL, NULL);
		SDL_RenderPresent(pRenderer);
		SDL_Delay(20);
		printf("END LOOP !\n\n");
	}
	LibererMap(carte);
	SDL_Quit();
}