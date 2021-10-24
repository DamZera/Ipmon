#include "jeu.h"

int test = 0;
Dresseur* joueur;

pthread_mutex_t mutexListOfPlayer = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutexNewPosition = PTHREAD_MUTEX_INITIALIZER;

void initClientContext() 
{
    LOG_DBG("to implement");
}

void jeuLockListOfPlayers()
{
    pthread_mutex_lock(&mutexListOfPlayer);
}

void jeuUnlockListOfPlayers()
{
    pthread_mutex_unlock(&mutexListOfPlayer);
}

void jeuDeplacement(SDL_Rect perso, Coord *persoAvant){
    char buf[BUFFER_SIZE];
    bzero(buf,BUFFER_SIZE);

    LOG_DBG("perso.x %d perso.y %d persoAvant->x %d persoAvant->y %d", perso.x, perso.y, persoAvant->x, persoAvant->y);
    if(perso.x != persoAvant->x || perso.y != persoAvant->y){
        pthread_mutex_lock(&mutexNewPosition);
        persoAvant->y = perso.y;
        persoAvant->x = perso.x;
        pthread_mutex_unlock(&mutexNewPosition);
    }
}

void jeuInitjoueur(Dresseur* joueurInit){
    joueur = joueurInit;
}

void setListOfPlayers(TTF_Font *police, char* buffer, ClientContext* cltCtx)
{
    char pseudo [BUFFER_SIZE];
    char* token = NULL;
    char* fieldsInToken = NULL;
    Dresseur_aff* listOfPlayers = NULL;

    SDL_Surface* texte = NULL;
    SDL_Rect perso, positiontexte;
    perso.w = 25;
    perso.h = 25;
    positiontexte = perso;
    
    //dresseur_list_jeu = vider_dresseur_liste(dresseur_list_jeu);

    token = strsep(&buffer, "+");
    while (token != NULL && strcmp(token, "\0") != 0) // TODO change with strncmp
    {
        fieldsInToken = strsep(&token, ":");
        if (fieldsInToken != NULL && strcmp(token, "\0") != 0) // TODO change with strncmp
        {
            strcpy(pseudo, fieldsInToken);
            fieldsInToken = strsep(&token, ":");
            perso.x = strtol(fieldsInToken, NULL, 10);
            fieldsInToken = strsep(&token, ":");
            perso.y = strtol(fieldsInToken, NULL, 10);
            
            listOfPlayers = ajouter_dresseur_aff(listOfPlayers, &perso, &positiontexte, texte);
            bzero(pseudo, BUFFER_SIZE);
        }
        token = strsep(&buffer, "+");
    }

    pthread_mutex_lock(&mutexListOfPlayer);
    cltCtx->listOfPlayer = vider_dresseur_liste(cltCtx->listOfPlayer);
    cltCtx->listOfPlayer = listOfPlayers;
    pthread_mutex_unlock(&mutexListOfPlayer);
}

Dresseur_aff* ajouter_dresseur_aff(Dresseur_aff *dresseur_list_jeu, SDL_Rect *perso, SDL_Rect *positionTexte, SDL_Surface *texte){

    Dresseur_aff* dresseur = malloc(sizeof(Dresseur_aff));
 
    dresseur->texte = texte;
    dresseur->perso = *perso;
    dresseur->positiontexte = *positionTexte;
    dresseur->next = NULL;
 
    if(dresseur_list_jeu == NULL){
        return dresseur;
    }
    else
    {
        Dresseur_aff* tmp = dresseur_list_jeu;
        while(tmp->next != NULL)
        {
            tmp = tmp->next;
        }
        tmp->next = dresseur;
        return dresseur_list_jeu;
    }
}

Dresseur_aff* vider_dresseur_liste(Dresseur_aff *dresseur_list_jeu){
    Dresseur_aff* dresseur = dresseur_list_jeu;
    Dresseur_aff* tmp;
    
    
    if(dresseur != NULL){
        while(dresseur->next != NULL){
            tmp = dresseur;
            dresseur = dresseur->next;
            free(tmp);
        }
        free(dresseur);
    }
    return NULL;
}
int jeuCombat(int sock)
{
    int combat = 0, idpokemonAdv = 0, end = 0, msg;
    char buf[BUFFER_SIZE]; bzero(buf,BUFFER_SIZE);
    //char* token;
    
    combat = rand()%300;
    
    if(combat<3){
        //idpokemonAdv = rand()%50+1;
        //printf("Combat contre pokemon id = %d\n",idpokemonAdv);
        /*send(sock,buf,strlen(buf),0);
        bzero(buf,BUFFER_SIZE);
        printf("Socket = %d\n",sock );
        bzero(buf,BUFFER_SIZE);
        while((end == 0) && (recv(sock,buf,BUFFER_SIZE,0))){
            printf("buff : %s",buf);
            Ipmon *ipmon_adv = malloc(sizeof(Ipmon));
            token = strtok(buf, ":");
            printf("Buf : %s\n",buf);
            if(strcmp(token,"msg") == 0){
                token = strtok(NULL, ":");
                msg = strtol(token,NULL, 10);
                send(sock,"recu",strlen("recu"),0);
            } else if(strcmp(buf,"end") == 0){
                end = 1;
            }
            else{
                printf("%s",buf);
                send(sock,"recu",strlen("recu"),0);
            }
            affichage_combat(msg, sock);
            bzero(buf,BUFFER_SIZE);
        }*/
    }
}

int affichage_combat(int msg, int sock){
    
    printf("STUBED must be reworked");
    //char buf[BUFFER_SIZE];
    //char* token;
    //int choix_ipmon;
    //Ipmon *ipmon = joueur->ipmons;
    //Ipmon *ipmon_adv;
    //joueur->niveau = 1;

    // switch(msg){
    //     case 1:    printf("**************************************\n");
    //             printf("Dresseur : \n\tPseudo : %s\n\tNiveau : %d\n", joueur->pseudo, joueur->niveau);

    //             printf("Liste de tous les ipmons du dresseur\n");
    //             while(ipmon != NULL){
    //                 printf("\n\nIpmon : \n\tId : %d\n\tNom : %s\n\tEtat : %s\n\tType : %s\n\tPoint de vie : %d\n\tAgilité : %d\n\tNiveau : %d\n\tPuissance attaque : %d\n\tNom attaque : %s\n\tPuissance défense : %d\n\t", ipmon->id, ipmon->nom, ipmon->etat, ipmon->type, ipmon->pv, ipmon->agilite, ipmon->niveau, ipmon->puissance_attaque, ipmon->nom_attaque, ipmon->puissance_defense);
    //                 ipmon = ipmon->next;
    //             }
    //             printf("Entrer l'id du pokemon choisi :\n");
    //         scanf("%d",&choix_ipmon);
    //         sprintf(buf,"%d",choix_ipmon);
    //         send(sock,buf,strlen(buf),0);
    //         break;
    //     case 2:
    //         recv(sock,buf,BUFFER_SIZE,0);
    //         token = strtok(buf, ":");
    //         ipmon_adv->id = strtol(token,NULL,10);
    //         token = strtok(NULL, ":");
    //         ipmon_adv->nom = token;
    //         token = strtok(NULL, ":");
    //         ipmon_adv->etat = token;
    //         token = strtok(NULL, ":");
    //         ipmon_adv->type = token;
    //         token = strtok(NULL, ":");
    //         ipmon_adv->typeEntier = strtol(token,NULL,10);
    //         token = NULL;


    //         bzero(buf,BUFFER_SIZE);
    //         send(sock,"recu",strlen("recu"),0);
    //         recv(sock,buf,BUFFER_SIZE,0);
    //         token = strtok(buf, ":");
    //         ipmon_adv->pv = strtol(token,NULL,10);
    //         token = strtok(NULL, ":");
    //         ipmon_adv->agilite = strtol(token,NULL,10);
    //         token = strtok(NULL, ":");
    //         ipmon_adv->niveau = strtol(token,NULL,10);
    //         token = strtok(NULL, ":");
    //         ipmon_adv->puissance_attaque = strtol(token,NULL,10);
    //         token = strtok(NULL, ":");
    //         ipmon_adv->nom_attaque = token;
    //         token = NULL;

    //         bzero(buf,BUFFER_SIZE);
    //         send(sock,"recu",strlen("recu"),0);
    //         recv(sock,buf,BUFFER_SIZE,0);
    //         token = strtok(buf, ":");
    //         ipmon_adv->precision_attaque = strtol(token,NULL,10);
    //         token = strtok(NULL, ":");
    //         ipmon_adv->puissance_defense = strtol(token,NULL,10);
    //         token = strtok(NULL, ":");
    //         ipmon_adv->nom_defense = token;
    //         token = strtok(NULL, ":");
    //         ipmon_adv->esquive = strtol(token,NULL,10);
    //         token = strtok(NULL, ":");
    //         ipmon_adv->precision = strtol(token,NULL,10);
    //         token = NULL;

    //         bzero(buf,BUFFER_SIZE);
    //         send(sock,"recu",strlen("recu"),0);
    //         recv(sock,buf,BUFFER_SIZE,0);
    //         token = strtok(buf, ":");
    //         ipmon_adv->puissance_attaque_special = strtol(token,NULL,10);
    //         token = strtok(NULL, ":");
    //         ipmon_adv->precision_attaque_special = strtol(token,NULL,10);
    //         token = strtok(NULL, ":");
    //         ipmon_adv->nom_attaque_special = token;
    //         token = strtok(NULL, ":");
    //         ipmon_adv->puissance_defense_special = strtol(token,NULL,10);
    //         token = strtok(NULL, ":");
    //         ipmon_adv->nom_defense_special = token;
    //         token = NULL;

    //         bzero(buf,BUFFER_SIZE);
    //         printf("\nIpmon sauvage : \n\tId : %d\n\tNom : %s\n\tEtat : %s\n\tType : %s\n\tPoint de vie : %d\n\tAgilité : %d\n\tNiveau : %d\n\tPuissance attaque : %d\n\tNom attaque : %s\n\tPuissance défense : %d\n\tNom défense : %s\n\t", ipmon_adv->id, ipmon_adv->nom, ipmon_adv->etat, ipmon_adv->type, ipmon_adv->pv, ipmon_adv->agilite, ipmon_adv->niveau, ipmon_adv->puissance_attaque, ipmon_adv->nom_attaque, ipmon_adv->puissance_defense, ipmon_adv->nom_defense);
    //         printf("Entrer l'id du pokemon choisi :\n");
    //         scanf("%d",&choix_ipmon);
    //         sprintf(buf,"%d",choix_ipmon);
    //         send(sock,buf,strlen(buf),0);
    //         break;
    //     case 3:
    //         printf("Appuyer: 1 pour faire une attaque simple, 2 pour une attaque spéciale et 0 pour abandonner!\n");
    //         scanf("%d", &choix_ipmon);
    //         sprintf(buf,"%d",choix_ipmon);
    //         send(sock,buf,strlen(buf),0);
    //         break;
    //     case 4:
    //         printf("Si vous abandonnez, vous allez perdre le combat; Voulez-vous quitter ? y/n\n");
    //         char c;
    //         scanf("%c", &c);
    //         sprintf(buf,"%c", c);
    //         send(sock,buf,strlen(buf),0);
    //     default:

    //         break;
    // }
}

Ipmon* ajouterIpmon(Ipmon* ipmon_list,int id,char* nom,char* etat,char* type,int typeEntier,int pv,int agilite,int niveau,int puissance_attaque,char* nom_attaque,int precision_attaque,int puissance_defense,int esquive,int precision,int puissance_attaque_special,int precision_attaque_special,char* nom_attaque_special,int puissance_defense_special){
    Ipmon* ipmon = malloc(sizeof(Ipmon));
    
    ipmon->id=id;
    ipmon->nom = malloc(sizeof(char)*strlen(nom));
    ipmon->etat= malloc(sizeof(char)*strlen(etat));
    ipmon->type= malloc(sizeof(char)*strlen(type));
    ipmon->nom_attaque= malloc(sizeof(char)*strlen(nom_attaque));
    ipmon->nom_attaque_special= malloc(sizeof(char)*strlen(nom_attaque_special));
    ipmon->nom= nom;
    ipmon->etat= etat;
    ipmon->type= type;
    ipmon->typeEntier= typeEntier; //Valeur du type d'ipmon : intervient dans le calcul du succès ou non d'une attaque
    ipmon->pv= pv; //Point de vie
    ipmon->agilite = agilite; //Rapidité de reaction d'un ipmon
    ipmon->niveau= niveau; //Niveau de l'ipmon : intervient dans le calcul du degat subit par un ipmon [1 - 100]
    ipmon->puissance_attaque= puissance_attaque; //Puissance d'attaque simple
    ipmon->nom_attaque= nom_attaque;
    ipmon->precision_attaque= precision_attaque;//Précision de l'attaque simple
    ipmon->puissance_defense= puissance_defense; //Puissance de defense : intervient dans le calcul du degat subit par un ipmon
    ipmon->esquive= esquive; //Capacité d'esquive d'un ipmon : intervient dans le calcul du succès ou non d'une attaque = 100
    ipmon->precision =precision; //Précision de l'ipmon
    ipmon->puissance_attaque_special= puissance_attaque_special;//Puissance de l'attaque spéciale d'un ipmon
    ipmon->precision_attaque_special= precision_attaque_special;//Précision de l'attaque spéciale d'un ipmon
    ipmon->nom_attaque_special= nom_attaque_special;
    ipmon->puissance_defense_special= puissance_defense_special;//Puissance de la défense spéciale d'un ipmon : intervient lorsqu'un ipmon est attaqué par une attaque spéciale

    
     if(ipmon_list == NULL){
        return ipmon;
    }
    else
    {
        Ipmon* tmp = ipmon_list;
        while(tmp != NULL)
        {
            tmp = tmp->next;
        }
        tmp = ipmon;
        return ipmon_list;
    }
}

void ajoutIpmon(int socket_cli){
    LOG_DBG("NOT DEFINE !");
    /*int n, end = 0, i = 0;
    Ipmon* ipmon_adv;
    char* token;
    char buf[80];
    dresseur->ipmons = NULL;
    
    while((end == 0) && recv(socket_cli,buf,80,0)){
        if(strcmp(buf,"end") ==0){
            end = 1;
        }else{
            ipmon_adv =  malloc(sizeof(Ipmon));
            int id;
            char* nom;
            
            ipmon_adv->etat = malloc(sizeof(char)*50);
            ipmon_adv->type = malloc(sizeof(char)*50);
            ipmon_adv->nom_attaque = malloc(sizeof(char)*50);
            ipmon_adv->nom_attaque_special = malloc(sizeof(char)*50);
            LOG_DBG("buf :: %s pok  %d",buf,i);
            i++;
            token = strtok(buf, ":");
            ipmon_adv->id = strtol(token,NULL,10);
            token = strtok(NULL, ":");
            ipmon_adv->nom = malloc(sizeof(char)*strlen(token));
            ipmon_adv->nom = token;
            token = strtok(NULL, ":");
            ipmon_adv->etat = malloc(sizeof(char)*strlen(token));
            ipmon_adv->etat = token;
            token = strtok(NULL, ":");
            ipmon_adv->type = malloc(sizeof(char)*strlen(token));
            ipmon_adv->type = token;
            token = strtok(NULL, ":");
            ipmon_adv->typeEntier = strtol(token,NULL,10);;
            token = NULL;


            bzero(buf,80);
            send(socket_cli,"recu",strlen("recu"),0);
            recv(socket_cli,buf,80,0);
            token = strtok(buf, ":");
            ipmon_adv->pv = strtol(token,NULL,10);
            token = strtok(NULL, ":");
            ipmon_adv->agilite = strtol(token,NULL,10);
            token = strtok(NULL, ":");
            ipmon_adv->niveau = strtol(token,NULL,10);
            token = strtok(NULL, ":");
            ipmon_adv->puissance_attaque = strtol(token,NULL,10);
            token = strtok(NULL, ":");
            ipmon_adv->nom_attaque = malloc(sizeof(char)*strlen(token));
            ipmon_adv->nom_attaque = token;
            token = NULL;

            bzero(buf,80);
            send(socket_cli,"recu",strlen("recu"),0);
            recv(socket_cli,buf,80,0);
            token = strtok(buf, ":");
            ipmon_adv->precision_attaque = strtol(token,NULL,10);
            token = strtok(NULL, ":");
            ipmon_adv->puissance_defense = strtol(token,NULL,10);
            token = strtok(NULL, ":");
            ipmon_adv->esquive = strtol(token,NULL,10);
            token = strtok(NULL, ":");
            ipmon_adv->precision = strtol(token,NULL,10);
            token = NULL;

            bzero(buf,80);
            send(socket_cli,"recu",strlen("recu"),0);
            recv(socket_cli,buf,80,0);
            token = strtok(buf, ":");
            ipmon_adv->puissance_attaque_special = strtol(token,NULL,10);
            token = strtok(NULL, ":");
            ipmon_adv->precision_attaque_special = strtol(token,NULL,10);
            token = strtok(NULL, ":");
            ipmon_adv->nom_attaque_special = malloc(sizeof(char)*strlen(token));
            ipmon_adv->nom_attaque_special = token;
            token = strtok(NULL, ":");
            LOG_DBG("buf defence :: %s pok  %d",token,i);
            ipmon_adv->puissance_defense_special = strtol(token,NULL,10);
            token = NULL;
            bzero(buf,80);
            send(socket_cli,"recu",strlen("recu"),0);
            dresseur->ipmons = ajouterIpmon(dresseur->ipmons,ipmon_adv->id,ipmon_adv->nom,ipmon_adv->etat,ipmon_adv->type,ipmon_adv->typeEntier,ipmon_adv->pv,ipmon_adv->agilite,ipmon_adv->niveau,ipmon_adv->puissance_attaque,ipmon_adv->nom_attaque,ipmon_adv->precision_attaque,ipmon_adv->puissance_defense,ipmon_adv->esquive,ipmon_adv->precision,ipmon_adv->puissance_attaque_special,ipmon_adv->precision_attaque_special,ipmon_adv->nom_attaque_special,ipmon_adv->puissance_defense_special);
        }
    }*/
}


void *threadUpdatePositionAndListOfPlayer(void *data)
{
    ClientContext* cltCtx = (ClientContext*) data;

    struct sockaddr_in cliaddr;
    memset(&cliaddr, 0, sizeof(cliaddr));

    int len, n;
    char bufBig[BIG_BUFFER_SIZE];
    char buf[BUFFER_SIZE];
  
    len = sizeof(cliaddr);  //len is value/resuslt
  
    while (cltCtx->stopThread == false)
    {
        usleep(100000); // 100ms

        pthread_mutex_lock(&mutexNewPosition);
        sprintf(buf, "%d:%d:%d:%s", NEW_COORDINATES,
            cltCtx->xyPlayer->x, cltCtx->xyPlayer->y, cltCtx->player->map);
        pthread_mutex_unlock(&mutexNewPosition);

        LOG_DBG("send : %s", buf);
        sendto(cltCtx->socket, buf, strlen(buf), MSG_CONFIRM,
            (const struct sockaddr *) cltCtx->srvaddr, sizeof(*(cltCtx->srvaddr)));
        
        n = recvfrom(cltCtx->socket, (char *)bufBig, BIG_BUFFER_SIZE, 
                MSG_WAITALL, ( struct sockaddr *) &cliaddr, &len);
        if (n >= 0)
        {
            bufBig[n] = '\0';
            LOG_DBG("receive : %s", bufBig);
            setListOfPlayers(NULL, bufBig, cltCtx);
        }
        else
        {
            LOG_WARN("Timeout without any message receive");
        }

        memset(buf, 0, sizeof(buf));
        memset(bufBig, 0, sizeof(bufBig));
    }

    pthread_exit(NULL);
}