#include "GUI_ipmon.h"
#include "jeu.h"

#define SGN(X) (((X)==0)?(0):(((X)<0)?(-1):(1)))
#define ABS2(X) ((((X)<0)?(-(X)):(X)))
// Utilisation de 3 fonctions qui permettent le déplacement
Dresseur_aff *dresseur_list_jeu;
Dresseur *joueur;
SDL_Surface *dresseurHaut = NULL, *dresseurBas = NULL , *dresseurGauche = NULL, *dresseurDroite = NULL;

void DeplacerVecteur(Input* in,int* vx,int* vy,SDL_Rect* perso,SDL_Surface* screen,int xscroll,int yscroll, SDL_Surface **dresseurActuel,SDL_Surface* texte) // Deplacement du dresseur PETIT SOUCIS QUE JE N'ARRIVE PAS A REGLER LORS DE LACHEMENT DE BUTON J'AI TESTE AVEC UNE BOUCLE WHILE MAIS ECRAN NOIR!!!
{	
	Dresseur_aff* dresseur = dresseur_list_jeu;

	SDL_Event event;
	SDL_Rect positionsurecran = *perso, positiontexte = *perso;
	SDL_Rect postionpersoB, texteB;
	positionsurecran.x -= xscroll;
	positionsurecran.y -= yscroll;
	positiontexte.x = positionsurecran.x - (texte->w /2) + (perso->w/2);
	positiontexte.y = positionsurecran.y -10;
	int vitesse = MOVE_SPEED_PLAYER;
	*vx = *vy = 0;

	SDL_EnableKeyRepeat(100, 100);

		UpdateEvents(in);
		
	while(dresseur != NULL){
		postionpersoB = dresseur->perso;
		texteB = dresseur->positiontexte;
		
		postionpersoB.x -= xscroll;
		postionpersoB.y -= yscroll;
		texteB.x -= xscroll;
		texteB.y -= yscroll;
		
		SDL_BlitSurface(dresseurBas, NULL, screen, &postionpersoB);
		SDL_BlitSurface(dresseur->texte, NULL, screen, &texteB);
		dresseur = dresseur->next;
	}
	

	if (in->key[SDLK_UP]){// si appuie sur la touche du haut
		*vy = -vitesse;
		*dresseurActuel=dresseurHaut;
		SDL_BlitSurface(dresseurHaut, NULL, screen, &positionsurecran);
		SDL_BlitSurface(texte, NULL, screen, &positiontexte);
	}
	else if (in->key[SDLK_DOWN]){
		*vy = vitesse;
		*dresseurActuel=dresseurBas;
		SDL_BlitSurface(dresseurBas, NULL, screen, &positionsurecran);
		SDL_BlitSurface(texte, NULL, screen, &positiontexte);
	}
	else if (in->key[SDLK_LEFT]){
		*vx = -vitesse;
		*dresseurActuel=dresseurGauche;
		SDL_BlitSurface(dresseurGauche, NULL, screen, &positionsurecran);
		SDL_BlitSurface(texte, NULL, screen, &positiontexte);
	}
	else if (in->key[SDLK_RIGHT]){
		*vx = vitesse;
		*dresseurActuel=dresseurDroite;
		SDL_BlitSurface(dresseurDroite, NULL, screen, &positionsurecran);
		SDL_BlitSurface(texte, NULL, screen, &positiontexte);	
	}
		SDL_EnableKeyRepeat(0, 0);
		SDL_BlitSurface(*dresseurActuel, NULL, screen, &positionsurecran);
		SDL_BlitSurface(texte, NULL, screen, &positiontexte);
}

int EssaiDeplacement(Map* carte,SDL_Rect* perso,int vx,int vy, int sock,SDL_Surface* screen)
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

void Deplace(Map* carte,SDL_Rect* perso,int vx,int vy,int LARGEUR_TILE,int HAUTEUR_TILE,int sock,SDL_Surface* screen)
{
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
}

void init_dresseur(){
	dresseurHaut = SDL_LoadBMP("./images/bmp/dresseurhaut.bmp");
	dresseurBas = SDL_LoadBMP("./images/bmp/dresseurbas.bmp");
	dresseurGauche = SDL_LoadBMP("./images/bmp/dresseurgauche.bmp");
	dresseurDroite = SDL_LoadBMP("./images/bmp/dresseurdroite.bmp");

	SDL_SetColorKey(dresseurBas, SDL_SRCCOLORKEY, SDL_MapRGB(dresseurBas->format, 0, 255, 0));// enlever le fond blanc des .bmp
	SDL_SetColorKey(dresseurHaut, SDL_SRCCOLORKEY, SDL_MapRGB(dresseurHaut->format, 0, 255, 0));
	SDL_SetColorKey(dresseurGauche, SDL_SRCCOLORKEY, SDL_MapRGB(dresseurGauche->format, 0, 255, 0));
	SDL_SetColorKey(dresseurDroite, SDL_SRCCOLORKEY, SDL_MapRGB(dresseurDroite->format, 0, 255, 0));
}

//DEFAULT nom_map "tilesetIPMON.txt" persox "300" persoy "150"
void jeu(char* nom_map,int persox, int persoy, int sock, char* pseudo,Dresseur *dresseur){
	joueur = dresseur;
	
	struct timeval tv1,tv2;
	long diff;

	SDL_Rect perso;
	Coord *persoAvant = malloc(sizeof(Coord));
	SDL_Surface* screen = NULL;
	SDL_Surface* texte = NULL; 
	SDL_Surface* dresseurActuel = SDL_LoadBMP("./images/bmp/dresseurbas.bmp");
	SDL_SetColorKey(dresseurActuel, SDL_SRCCOLORKEY, SDL_MapRGB(dresseurActuel->format, 0, 255, 0));
	dresseur_list_jeu = NULL;
	
	TTF_Font *police = NULL;
   	SDL_Color couleurNoire = {0, 0, 0}; //couleur noir
	TTF_Init();
	police = TTF_OpenFont("./arial.ttf", 12); // police
	texte = TTF_RenderText_Blended(police, pseudo, couleurNoire); // ecriture du texte (exemple)
	
	SDL_Event event;
	Map* carte;
	//carte = malloc(sizeof(Map));
	Input in;
	int LARGEUR_TILE,HAUTEUR_TILE;
	int vx = 0,vy = 0;
	LARGEUR_TILE = 25;
	HAUTEUR_TILE = 25;
	memset(&in,0,sizeof(in));
		
	screen = SDL_SetVideoMode(LARGEUR_FENETRE,HAUTEUR_FENETRE,32,SDL_HWSURFACE|SDL_DOUBLEBUF);
	carte = ChargerMap(nom_map,LARGEUR_FENETRE,HAUTEUR_FENETRE);
	perso.x = persox;
	perso.y = persoy;
	persoAvant->x = persox;
	persoAvant->y = persoy;
	perso.w = 25;
	perso.h = 25;
	
	gettimeofday(&tv1,NULL);
	gettimeofday(&tv2,NULL);
	
	init_dresseur();
	
	srand(time(NULL));

	jeuInitjoueur(joueur);

	while(!in.key[SDLK_ESCAPE]) //tant qu 'on a pas appuyé sur ESCAPE pour quitter
	{	
		gettimeofday(&tv1,NULL);
		diff= (tv1.tv_sec - tv2.tv_sec);
		//printf("Diff :: %06ld \n",diff);
		jeuDeplacement(perso,persoAvant,sock);
		UpdateEvents(&in);
		Deplace(carte,&perso,vx,vy,LARGEUR_TILE,HAUTEUR_TILE,sock, screen);
		FocusScrollCenter(carte,&perso);
		AfficherMap(carte,screen,carte->xscroll,carte->yscroll);
		DeplacerVecteur(&in,&vx,&vy,&perso,screen,carte->xscroll,carte->yscroll, &dresseurActuel,texte);
		if(diff>0){
			dresseur_list_jeu = jeuAfficherDresseurs(sock,nom_map,police);
			gettimeofday(&tv2,NULL);
		}
		SDL_Flip(screen);
		SDL_Delay(5);
	}
	LibererMap(carte);
	SDL_Quit();
}