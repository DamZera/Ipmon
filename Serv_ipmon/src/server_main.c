#include "server_main.h"
#include "mysql_ipmon.h"
#include "jeu_serv.h"

#include <errno.h>

#include "logger.h"

#define TAILLE_BUFF 200

/*Mutex pour les zones critiques*/
pthread_mutex_t globalMutex = PTHREAD_MUTEX_INITIALIZER;

Dresseur* chercher_dresseur(Dresseur *dresseur_list,int socket){
    // TODO fix after work on IPMON
    /*Dresseur* dresseur = dresseur_list;
    while(dresseur != NULL){
        if(dresseur->sock == socket)
            return dresseur;
        else
            dresseur = dresseur->next;
    }

    return NULL;*/
}

Dresseur* addPlayer(Dresseur *dresseur_list, struct sockaddr_in* cliaddr, char* pseudo, int coodX, int coodY, char* map, int id){

    Dresseur* dresseur = malloc(sizeof(Dresseur));
 
    dresseur->id = id;

    // Copy socket addr
    dresseur->cliaddr.sin_family = AF_INET;
    dresseur->cliaddr.sin_port = cliaddr->sin_port;
    dresseur->cliaddr.sin_addr.s_addr = cliaddr->sin_addr.s_addr;

    dresseur->pseudo = (char*)malloc(sizeof(char)*strlen(pseudo)+1);
    dresseur->map = (char*)malloc(sizeof(char)*strlen(map)+1);
    strcpy(dresseur->pseudo, pseudo);
    strcpy(dresseur->map, map);
    dresseur->next = NULL;
    dresseur->coodX = coodX;
    dresseur->coodY = coodY;
 
    if(dresseur_list == NULL){
        return dresseur;
    }
    else
    {
        Dresseur* tmp = dresseur_list;
        while(tmp->next != NULL)
        {
            tmp = tmp->next;
        }
        tmp->next = dresseur;
        return dresseur_list;
    }
}

Ipmon* cree_Ipmon(int id, char* nom, char* etat, char* type, int typeEntier     , int pv     , int agilite , int niveau     , int puissance_attaque , char* nom_attaque, int precision_attaque, int puissance_defense , char* nom_defense, int esquive     , int precision     , int puissance_attaque_special, int precision_attaque_special, char* nom_attaque_special, int puissance_defense_special    , char* nom_defense_special){
    Ipmon* ipmon = malloc(sizeof(Ipmon));

    ipmon->id = id;
    snprintf(ipmon->nom, MAX_SIZE_IPMON_STR, "%s", nom);
    snprintf(ipmon->etat, MAX_SIZE_IPMON_STR, "%s", etat);
    snprintf(ipmon->type, MAX_SIZE_IPMON_STR, "%s", type);
    ipmon->typeEntier = typeEntier; 
    ipmon->pv = pv; 
    ipmon->agilite = agilite; 
    ipmon->niveau = niveau; 
    ipmon->puissance_attaque = puissance_attaque; 
    snprintf(ipmon->nom_attaque, MAX_SIZE_IPMON_STR, "%s", nom_attaque);
    ipmon->precision_attaque = precision_attaque;
    ipmon->puissance_defense = puissance_defense; 
    snprintf(ipmon->nom_defense, MAX_SIZE_IPMON_STR, "%s", nom_defense);
    ipmon->esquive = esquive; 
    ipmon->precision = precision;
    ipmon->puissance_attaque_special = puissance_attaque_special;
    ipmon->precision_attaque_special = precision_attaque_special;
    snprintf(ipmon->nom_attaque_special, MAX_SIZE_IPMON_STR, "%s", nom_attaque_special);
    ipmon->puissance_defense_special = puissance_defense_special;
    snprintf(ipmon->nom_defense_special, MAX_SIZE_IPMON_STR, "%s", nom_defense_special);

    return ipmon;
}

void afficher_dresseur(Dresseur *dresseur_list){
    Dresseur* dresseur = dresseur_list;
    int i = 0;
    LOG_INFO("Liste des dresseurs connecté :");
    while(dresseur != NULL){
        LOG_INFO(" Dresseur %d : %s map : %s cood %d:%d",
            i, dresseur->pseudo, dresseur->map, dresseur->coodX, dresseur->coodY);
        dresseur = dresseur->next; i++;
    }
}


Dresseur* supprimer_dresseur(Dresseur *dresseur_list, int socket){
    
    LOG_ERR("TO REFACTOR THIS PART");
    /*if(dresseur_list == NULL)
        return NULL;
        
    if(dresseur_list->sock == socket){
        Dresseur* dresseur = dresseur_list->next;
        free(dresseur_list);
            
        return dresseur;
    }else{
        dresseur_list->next = supprimer_dresseur(dresseur_list->next, socket);
        return dresseur_list;
    }*/
    return dresseur_list;
}

void envoieIpmonDresseur(int s_dial,MYSQL* ipmon_bdd, Dresseur* dresseur_list){
    Dresseur* dresseur = chercher_dresseur(dresseur_list, s_dial);
    char requete[150];
    int i;
    char buf[MAX_SIZE_IPMON_STR];
    bzero(buf,MAX_SIZE_IPMON_STR);
    Ipmon* ipmon_adv = malloc(sizeof(Ipmon));
    MYSQL_RES *result;
    MYSQL_ROW row;

    sprintf(requete,"SELECT * FROM ipdex,ipmon WHERE ipdex.dresseur = '%d' AND ipdex.ipmon = ipmon.Id_IPmon", dresseur->id);
    LOG_INFO("%s",requete);

    if(mysql_query(ipmon_bdd, requete)){
                finish_with_error(ipmon_bdd);
    } 
    result = mysql_store_result(ipmon_bdd);
    row = mysql_fetch_row(result);
    while(row){
        for(i=0;i<20;i++)
            LOG_DBG("%s ",row[i]);
        ipmon_adv->id = strtol(row[3],NULL,10);ipmon_adv->typeEntier = strtol(row[7],NULL,10);
        
        snprintf(ipmon_adv->nom, MAX_SIZE_IPMON_STR, "%s", row[4]);
        snprintf(ipmon_adv->etat, MAX_SIZE_IPMON_STR, "%s", row[5]);
        snprintf(ipmon_adv->type, MAX_SIZE_IPMON_STR, "%s", row[6]);
        snprintf(ipmon_adv->nom_attaque, MAX_SIZE_IPMON_STR, "%s", row[12]);
        snprintf(ipmon_adv->nom_attaque_special, MAX_SIZE_IPMON_STR, "%s", row[18]);
        
        
        ipmon_adv->pv = strtol(row[8],NULL,10);ipmon_adv->agilite = strtol(row[9],NULL,10);ipmon_adv->niveau = strtol(row[1],NULL,10);ipmon_adv->puissance_attaque = strtol(row[10],NULL,10);
    
        ipmon_adv->precision_attaque = strtol(row[11],NULL,10);ipmon_adv->puissance_defense = strtol(row[13],NULL,10);ipmon_adv->esquive = strtol(row[14],NULL,10);ipmon_adv->precision = strtol(row[15],NULL,10);
    
        ipmon_adv->puissance_attaque_special = strtol(row[16],NULL,10);ipmon_adv->precision_attaque_special = strtol(row[17],NULL,10);ipmon_adv->puissance_defense_special = strtol(row[19],NULL,10);

        // MY EYES please need rework

        // printf("%d:%s:%s:%s:%d\n",ipmon_adv->id,ipmon_adv->nom,ipmon_adv->etat,ipmon_adv->type,ipmon_adv->typeEntier);
        // sprintf(buf,"%d:%s:%s:%s:%d",ipmon_adv->id,ipmon_adv->nom,ipmon_adv->etat,ipmon_adv->type,ipmon_adv->typeEntier);
        // send(s_dial,buf,MAX_SIZE_IPMON_STR,0);
        // bzero(buf,MAX_SIZE_IPMON_STR);
        // recv(s_dial,buf,MAX_SIZE_IPMON_STR,0);
        // bzero(buf,MAX_SIZE_IPMON_STR);
        // sprintf(buf,"%d:%d:%d:%d:%s",ipmon_adv->pv,ipmon_adv->agilite,ipmon_adv->niveau,ipmon_adv->puissance_attaque,ipmon_adv->nom_attaque);
        // send(s_dial,buf,MAX_SIZE_IPMON_STR,0);
        // bzero(buf,MAX_SIZE_IPMON_STR);
        // recv(s_dial,buf,MAX_SIZE_IPMON_STR,0);
        // bzero(buf,MAX_SIZE_IPMON_STR);
        // sprintf(buf,"%d:%d:%d:%d",ipmon_adv->precision_attaque,ipmon_adv->puissance_attaque,ipmon_adv->esquive,ipmon_adv->precision);
        // send(s_dial,buf,MAX_SIZE_IPMON_STR,0);
        // bzero(buf,MAX_SIZE_IPMON_STR);
        // recv(s_dial,buf,MAX_SIZE_IPMON_STR,0);
        // bzero(buf,MAX_SIZE_IPMON_STR);
        // sprintf(buf,"%d:%d:%s:%d",ipmon_adv->puissance_attaque_special,ipmon_adv->precision_attaque_special,ipmon_adv->nom_attaque_special,ipmon_adv->puissance_defense_special);
        // send(s_dial,buf,MAX_SIZE_IPMON_STR,0);
        // bzero(buf,MAX_SIZE_IPMON_STR);
        // recv(s_dial,buf,MAX_SIZE_IPMON_STR,0);
        row = mysql_fetch_row(result);
    }
    send(s_dial,"end",strlen("end"),0);
}


void newPositionOfPlayer(int coodX, int coodY, struct sockaddr_in* cliaddr, ServerThreadContext* servCtx)
{
    Dresseur* dresseur = servCtx->dresseursList;

    //LOG_DBG("Enter newPosition %d:%d %d", coodX, coodY, cliaddr->sin_port);
    
    while(dresseur != NULL)
    {
        if((dresseur->cliaddr.sin_addr.s_addr == cliaddr->sin_addr.s_addr)
            && (dresseur->cliaddr.sin_port == cliaddr->sin_port))
        {
            dresseur->coodX = coodX;
            dresseur->coodY = coodY;
            break;
        }
        dresseur = dresseur->next;
    }
}

void sendPlayers(struct sockaddr_in* cliaddr, char *map, ServerThreadContext* servCtx)
{
    char buf[BIG_BUFFER_SIZE];
    char low_buff[BUFFER_SIZE];
    bzero(low_buff, BUFFER_SIZE);
    bzero(buf,BIG_BUFFER_SIZE);

    int err = ERR_NOERROR;

    Dresseur* dresseur = servCtx->dresseursList;

    LOG_DBG("sendPlayers START with map %s", map);

    while(dresseur != NULL)
    {
        if(strcmp(dresseur->map, map) == 0 
            && (dresseur->cliaddr.sin_addr.s_addr != cliaddr->sin_addr.s_addr)
            && (dresseur->cliaddr.sin_port != cliaddr->sin_port))
        {
            sprintf(low_buff,"%s:%d:%d",dresseur->pseudo,dresseur->coodX,dresseur->coodY);
            bzero(low_buff,BUFFER_SIZE);
            strcat(buf, low_buff);
            //recv(*s_dial,buf,BUFFER_SIZE,0);
            //printf("Sock : %d buf : /%s/",*s_dial,buf);
        }
        //bzero(buf,BUFFER_SIZE);
        dresseur = dresseur->next;
        if (dresseur != NULL)
        {
            strcat(buf, "+");
        }
        
    }

    err = sendto(servCtx->socketServer, buf, strlen(buf),
        MSG_CONFIRM, (const struct sockaddr *) cliaddr, sizeof(*cliaddr));

    if (err == -1)
    {
        LOG_ERR("sendto : %s", strerror(errno));
    }
    //send(*s_dial,"000end",strlen("000end"),0);
    //LOG_DBG("sendPlayers END");
}

int connectToIPMON(struct sockaddr_in* cliaddr,
    MYSQL* ipmon_bdd, char* pseudo, char* pass, ServerThreadContext* servCtx)
{
    char buf [TAILLE_BUFF];
    int connecte = 0;
    int err = ERR_NOERROR;

    Dresseur* dresseur = NULL;

    dresseur = connectToIPMONSQL(ipmon_bdd, pseudo, pass);

    if (dresseur != NULL)
    {
        // Prepare message for client
        sprintf(buf,"%d:%d:%d:%s", ACCEPT_CONNECTION, dresseur->coodX, dresseur->coodY, dresseur->map);
        servCtx->dresseursList = addPlayer(servCtx->dresseursList,
            cliaddr, pseudo, dresseur->coodX, dresseur->coodY, dresseur->map, 1);
        free(dresseur);
        afficher_dresseur(servCtx->dresseursList);
        connecte = 1;
    }
    else
    {
        sprintf(buf,"%d", REFUSE_CONNECTION);
    }
    LOG_DBG("dresseur CONNECT = %d buf = %s", connecte, buf);
    err = sendto(servCtx->socketServer, buf, strlen(buf),
        MSG_CONFIRM, (const struct sockaddr *) cliaddr, sizeof(*cliaddr));

    if (err == -1)
    {
        LOG_ERR("sendto : %s", strerror(errno));
    }

    return connecte;
}

int createUdpSocket(int port, int nb_max_clients) {
  unsigned int socketServer;
  struct sockaddr_in serv_addr;

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = htonl (INADDR_ANY);
  serv_addr.sin_port = htons (port);
  memset (&serv_addr.sin_zero, 0, sizeof(serv_addr.sin_zero));

  socketServer = socket (AF_INET, SOCK_DGRAM, 0);
  bind (socketServer, (struct sockaddr *)&serv_addr, sizeof serv_addr);

  return socketServer;
}

void processItemsInCircularBuffer(ServerThreadContext* servCtx)
{
    char* message = NULL;
    char* token = NULL;
    char pseudo[200] = "";
    char pass[200] = "";
    int code = CODE_INVALID;
    
    MYSQL *ipmon_bdd;
    struct sockaddr_in cliaddr;

    ipmon_bdd = connect_bdd();

    // Main server loop for all clients
    while (1) 
    {
        memset(pass, 0, 200);
        memset(pseudo, 0, 200);
        code = CODE_INVALID;
        sleep(1);
        if (ERR_NOERROR == circularBufEmpty(servCtx->circBuffer))
        {
            circularBufRead(servCtx->circBuffer, &message, &cliaddr);
            LOG_DBG("Consumer receive : %s", message);

            token = strsep(&message, ":");

            if (token != NULL && message != NULL)
            {
                code = strtol(token, NULL, 10);
                LOG_DBG("code : %d", code);
                /*************************************
                * CONNECTION OR REGISTER
                **************************************/
                if (code == CONNECT_TO_IPMON || code == REGISTER_TO_IPMON)
                {
                    token = strsep(&message, ":");
                    if (token != NULL)
                    {
                        strncpy(pseudo, token, 200);
                        token = strsep(&message, ":");
                        if (token != NULL)
                        {
                            strncpy(pass, token, 200);
                        }
                    }
                    
                    if (code == CONNECT_TO_IPMON)
                    {
                        if (connectToIPMON(&cliaddr, ipmon_bdd, pseudo, pass, servCtx))
                        {
                            // TODO change state in dresseur structure
                            LOG_DBG("Connecion sucess !");
                        }
                    }
                    else
                    {
                        insertDresseurSQL(ipmon_bdd, pseudo, pass);
                    }
                }
                /*************************************
                * IN GAME : sendNewCood and Dresseurs
                **************************************/
                else if (code == NEW_COORDINATES)
                {
                    int x = 0;
                    int y = 0;

                    token = strsep(&message, ":");
                    if (token == NULL)
                        break;
                    x = strtol(token, NULL, 10);
                    token = strsep(&message, ":");
                    if (token == NULL)
                        break;
                    y = strtol(token, NULL, 10);
                    newPositionOfPlayer(x, y, &cliaddr, servCtx);
                }
                else if (code == SEND_LIST_PLAYERS) {
                    // TODO fix second argument
                    sendPlayers(&cliaddr, strsep(&message, ":"), servCtx); 
                }
            }
            else
            {
                if (message != NULL && strtol(message, NULL,10) == CLOSE_CLIENT)
                {
                    code = CLOSE_CLIENT;
                }
            }

            message = NULL;
        }
    }
}

void *threadDownload(void *data)
{
    ServerThreadContext* servCtx = (ServerThreadContext*) data;

    struct sockaddr_in cliaddr;
    memset(&cliaddr, 0, sizeof(cliaddr));

    int len, n;
    char buffer[1024];
    char ip[24];
  
    len = sizeof(cliaddr);  //len is value/resuslt
  
    while (n = recvfrom(servCtx->socketServer, (char *)buffer, 1024, 
                MSG_WAITALL, ( struct sockaddr *) &cliaddr, &len))
    {
        buffer[n] = '\0';
        inet_ntop(AF_INET, &(((struct sockaddr_in*)&cliaddr)->sin_addr),
                    ip, 24);
        LOG_DBG("Client[%s] : %s", ip, buffer);
        circularBufPut(servCtx->circBuffer, buffer, &cliaddr);
        // sendto(servCtx->socketServer, "msg receive ;)\n", strlen("msg receive ;)"), 
        //     MSG_CONFIRM, (const struct sockaddr *) &cliaddr, len);
        memset(&cliaddr, 0, sizeof(cliaddr));
        memset(&buffer, 0, sizeof(buffer));
        memset(&ip, 0, strlen(ip));
    }
}


int main (int argc, char* argv[])
{
    ServerThreadContext* servCtx = (ServerThreadContext*)malloc(sizeof(ServerThreadContext));
    servCtx->circBuffer = createCircularBuf(64);
    servCtx->dresseursList = NULL;

    // Create socket with a max of MAX_CLIENTS
    servCtx->socketServer = createUdpSocket(strtol(argv[1],NULL,10), MAX_CLIENTS);

    pthread_attr_t *thread_attributes ;
    pthread_t tid;
    thread_attributes = malloc(sizeof *thread_attributes);
    pthread_attr_init(thread_attributes);
    pthread_attr_setdetachstate(thread_attributes, PTHREAD_CREATE_DETACHED);

    pthread_create(&tid, thread_attributes, threadDownload, servCtx);

    pthread_attr_destroy(thread_attributes);

    processItemsInCircularBuffer(servCtx);

    close(servCtx->socketServer);
    exit(EXIT_SUCCESS);
}
