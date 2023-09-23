#include "GUI_ipmon.h"
#include "jeu.h"

Dresseur *joueur;
SDL_Texture *dresseurHaut = NULL, *dresseurBas = NULL,
    *dresseurGauche = NULL, *dresseurDroite = NULL, *dresseurActuel = NULL;

void DeplacerVecteur(const Uint8* in, int* vx, int* vy, SDL_Rect* perso,
                    SDL_Renderer* pRenderer, int xscroll, int yscroll,
                    SDL_Texture* texte, ClientContext* cltCtx)
{    
    LOG_DBG("DeplacerVecteur :: START");

    SDL_Rect positionsurecran = *perso, positiontexte = *perso;
    SDL_Rect postionpersoB, texteB;

    SDL_Rect imgSize;
    imgSize.x = 0;
    imgSize.y = 0;

    SDL_Rect texteSize;
    texteSize.x = 0;
    texteSize.y = 0;

    SDL_QueryTexture(dresseurHaut, NULL, NULL, &imgSize.w, &imgSize.h);
    SDL_QueryTexture(texte, NULL, NULL, &texteSize.w, &texteSize.h);

    positionsurecran.x -= xscroll;
    positionsurecran.y -= yscroll;
    positiontexte.x = positionsurecran.x - (texteSize.w/2) + (imgSize.w/2);
    positiontexte.y = positionsurecran.y -10;
    positiontexte.w = texteSize.w;
    positiontexte.h = texteSize.h;
    *vx = *vy = 0;

    jeuLockListOfPlayers();
    Dresseur_aff* ptrListPlayers = cltCtx->listOfPlayer;
    while(ptrListPlayers != NULL){
        postionpersoB = ptrListPlayers->perso;
        texteB = ptrListPlayers->positiontexte;
        
        postionpersoB.x -= xscroll;
        postionpersoB.y -= yscroll;
        texteB.x -= xscroll;
        texteB.y -= yscroll;

        SDL_RenderCopy(pRenderer, dresseurBas, &imgSize, &postionpersoB);
        //SDL_BlitSurface(dresseur->texte, NULL, screen, &texteB);
        ptrListPlayers = ptrListPlayers->next;
    }
    jeuUnlockListOfPlayers();

    if (in[SDL_SCANCODE_UP]){
        *vy = -MOVE_SPEED_PLAYER;
        dresseurActuel=dresseurHaut;
    }
    else if (in[SDL_SCANCODE_DOWN]){
        *vy = MOVE_SPEED_PLAYER;
        dresseurActuel=dresseurBas;
    }
    else if (in[SDL_SCANCODE_LEFT]){
        *vx = -MOVE_SPEED_PLAYER;
        dresseurActuel=dresseurGauche;
    }
    else if (in[SDL_SCANCODE_RIGHT]){
        *vx = MOVE_SPEED_PLAYER;
        dresseurActuel=dresseurDroite;
    }

    SDL_RenderCopy(pRenderer, dresseurActuel, &imgSize, &positionsurecran);
    SDL_RenderCopy(pRenderer, texte, &texteSize, &positiontexte);
    LOG_DBG("DeplacerVecteur :: END");
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
        LOG_DBG("Apres CollisionDecor");
        
        return 0;
    }
    return 0;
}

void Deplace(Map* carte, SDL_Rect* perso, int vx, int vy,
    int LARGEUR_TILE,int HAUTEUR_TILE,int sock,SDL_Surface *screen)
{
    //LOG_DBG("Deplace :: START");
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
    //LOG_DBG("Deplace :: END");
}

void init_dresseur(SDL_Renderer* ren){
    SDL_Surface* dressHaut = SDL_LoadBMP("./images/bmp/dresseurhaut.bmp");
    SDL_Surface* dressBas = SDL_LoadBMP("./images/bmp/dresseurbas.bmp");
    SDL_Surface* dressGauche = SDL_LoadBMP("./images/bmp/dresseurgauche.bmp");
    SDL_Surface* dressDroite = SDL_LoadBMP("./images/bmp/dresseurdroite.bmp");

    // Delete white background and add alpha channel
    SDL_SetColorKey(dressBas, SDL_TRUE, SDL_MapRGB(dressBas->format, 0, 255, 0));
    SDL_SetColorKey(dressHaut, SDL_TRUE, SDL_MapRGB(dressHaut->format, 0, 255, 0));
    SDL_SetColorKey(dressGauche, SDL_TRUE, SDL_MapRGB(dressGauche->format, 0, 255, 0));
    SDL_SetColorKey(dressDroite, SDL_TRUE, SDL_MapRGB(dressDroite->format, 0, 255, 0));

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

void mainLoop(int sock, struct sockaddr_in* srvaddr, Dresseur *dresseur)
{
    //DEFAULT nom_map "tilesetIPMON.txt" persox "300" persoy "150"
    LOG_DBG("IN jeu function on map:%s", dresseur->map);
    joueur = dresseur;
    
    struct timeval tv1,tv2;
    long elapsed; // ms

    if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) != 0) {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        exit(1);
    }

    SDL_Rect perso;
    Coord *persoAvant = malloc(sizeof(Coord));
    SDL_Surface* texteSurface = NULL;
    SDL_Texture* texteTexture = NULL;
    
    TTF_Font *police = NULL;
    SDL_Color blackColor = {0, 0, 0};
    TTF_Init();
    police = TTF_OpenFont("./arial.ttf", 10);
    texteSurface = TTF_RenderText_Solid(police, dresseur->pseudo, blackColor);

    Map* carte;
    //carte = malloc(sizeof(Map));
    int LARGEUR_TILE,HAUTEUR_TILE;
    int vx = 0,vy = 0;
    LARGEUR_TILE = 25;
    HAUTEUR_TILE = 25;

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
    texteTexture = SDL_CreateTextureFromSurface(pRenderer, texteSurface);
    SDL_FreeSurface(texteSurface);
    
    srand(time(NULL));

    jeuInitjoueur(joueur);

    const Uint8 *state = SDL_GetKeyboardState(NULL);

    ClientContext cltCtx;
    cltCtx.socket = sock;
    cltCtx.srvaddr = srvaddr;
    cltCtx.xyPlayer = persoAvant;
    cltCtx.player = dresseur;
    cltCtx.stopThread = false;
    cltCtx.listOfPlayer = NULL;

    pthread_attr_t *thread_attributes;
    pthread_t tid;
    thread_attributes = malloc(sizeof *thread_attributes);
    pthread_attr_init(thread_attributes);
    pthread_attr_setdetachstate(thread_attributes, PTHREAD_CREATE_DETACHED);

    pthread_attr_t *thread_attributes_B;
    pthread_t tid_B;
    thread_attributes_B = malloc(sizeof *thread_attributes);
    pthread_attr_init(thread_attributes_B);
    pthread_attr_setdetachstate(thread_attributes_B, PTHREAD_CREATE_DETACHED);

    pthread_create(&tid, thread_attributes, threadUpdatePositionAndListOfPlayer, &cltCtx);

    pthread_create(&tid_B, thread_attributes_B, threadSendPosition, &cltCtx);

    pthread_attr_destroy(thread_attributes);
    pthread_attr_destroy(thread_attributes_B);

    while(!state[SDL_SCANCODE_ESCAPE]) // Press ESCAPE to leave
    {    
        // 2 usec
        SDL_RenderClear(pRenderer);

        // Update all Keyboard events at 120 usec
        SDL_PumpEvents();

        jeuDeplacement(perso, persoAvant);

        // 10 usec
        Deplace(carte, &perso, vx, vy, LARGEUR_TILE, HAUTEUR_TILE, sock, screen);
        // 10 usec
        FocusScrollCenter(carte,&perso);

        // 450 usec
        AfficherMap(carte, pRenderer, carte->xscroll, carte->yscroll);

        // 25 usec
        DeplacerVecteur(state, &vx, &vy, &perso, pRenderer,
            carte->xscroll, carte->yscroll, texteTexture, &cltCtx);
        
        //SDL_UpdateTexture(sdlTexture, NULL, screen->pixels, screen->pitch);

        //SDL_RenderClear(pRenderer);
        //SDL_RenderCopy(pRenderer, sdlTexture, NULL, NULL);
        SDL_RenderPresent(pRenderer);
        SDL_Delay(20);
        LOG_DBG("END LOOP !");
    }

    cltCtx.stopThread = true;
    sleep(1);
    LibererMap(carte);
    TTF_Quit();
    SDL_Quit();
}