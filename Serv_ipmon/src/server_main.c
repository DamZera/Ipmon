#include "server_main.h"

#ifdef USE_MYSQL_LIB
#include "mysql_ipmon.h"
#endif
#include "jeu_serv.h"

#include <errno.h>

#include "logger.h"

#define TAILLE_BUFF 200

/*Mutex pour les zones critiques*/
pthread_mutex_t circBufferMutex = PTHREAD_MUTEX_INITIALIZER;

Dresseur* chercher_dresseur(Dresseur *dresseur_list, int socket){
    // TODO fix after work on IPMON
    /*Dresseur* dresseur = dresseur_list;
    while(dresseur != NULL){
        if(dresseur->sock == socket)
            return dresseur;
        else
            dresseur = dresseur->next;
    }*/

    return NULL;
}

Dresseur* searchDresseurPseudo(Dresseur *dresseur_list, char* pseudo){
    Dresseur* dresseur = dresseur_list;
    while(dresseur != NULL){
        if(strncmp(pseudo, dresseur->pseudo, MAX_SIZE_DRESSEUR_STR) == 0)
            return dresseur;
        else
            dresseur = dresseur->next;
    }

    return NULL;
}

Dresseur* addPlayer(Dresseur *dresseur_list, struct sockaddr_in* cliaddr, char* pseudo, int coodX, int coodY, char* map, int id){

    if (searchDresseurPseudo(dresseur_list, pseudo) != NULL)
    {
        LOG_INFO("Dresseur already exist ! : %s", pseudo);
        return dresseur_list;
    }
    else
    {
        Dresseur* dresseur = malloc(sizeof(Dresseur));

        dresseur->id = id;

        // Copy socket addr
        dresseur->cliaddr.sin_family = AF_INET;
        dresseur->cliaddr.sin_port = cliaddr->sin_port;
        dresseur->cliaddr.sin_addr.s_addr = cliaddr->sin_addr.s_addr;
        snprintf(dresseur->pseudo, MAX_SIZE_DRESSEUR_STR, "%s", pseudo);
        snprintf(dresseur->map, MAX_SIZE_DRESSEUR_STR, "%s", map);
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
    LOG_INFO("Dresseurs online :");
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

/*
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
*/

void newPositionOfPlayer(int coodX, int coodY, struct sockaddr_in* cliaddr, ServerThreadContext* servCtx)
{
    Dresseur* dresseur = servCtx->dresseursList;

    while(dresseur != NULL)
    {
        if((dresseur->cliaddr.sin_addr.s_addr == cliaddr->sin_addr.s_addr)
            && (dresseur->cliaddr.sin_port == cliaddr->sin_port))
        {
            // To many log
            // LOG_DBG("newPositionOfPlayer %d:%s -> %d:%d",
            //     cliaddr->sin_port, dresseur->pseudo, coodX, coodY);
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
    bzero(buf, BIG_BUFFER_SIZE);

    int err = ERR_NOERROR;

    Dresseur* dresseur = servCtx->dresseursList;

    while(dresseur != NULL)
    {
        if (NULL == map ||
            NULL == dresseur->map ||
            NULL == cliaddr ||
            NULL == dresseur->pseudo)
        {
            LOG_INFO("sendPlayers error map:%p dresseur->map:%p cliaddr:%p dresseur->pseudo:%p", map, dresseur->map, cliaddr, dresseur->pseudo);
        }
        else if(strcmp(dresseur->map, map) == 0 &&
            ((dresseur->cliaddr.sin_addr.s_addr != cliaddr->sin_addr.s_addr) ||
             (dresseur->cliaddr.sin_port != cliaddr->sin_port)))
        {
            bzero(low_buff, BUFFER_SIZE);
            LOG_DBG("sendPlayers add %x for %x", dresseur->cliaddr.sin_port, cliaddr->sin_port);
            snprintf(low_buff, BUFFER_SIZE, "%s:%d:%d+",dresseur->pseudo, dresseur->coodX, dresseur->coodY);
            strcat(buf, low_buff);
        }
        //bzero(buf,BUFFER_SIZE);
        dresseur = dresseur->next;
    }

    if ('\0' != buf[0])
    {
        LOG_DBG("sendPlayers to s_addr%x sin_port%x : %s", cliaddr->sin_addr.s_addr, cliaddr->sin_port, buf);
        err = sendto(servCtx->socketServer, buf, strlen(buf),
            MSG_CONFIRM, (const struct sockaddr *) cliaddr, sizeof(*cliaddr));

        if (err == -1)
        {
            LOG_ERR("sendto : %s", strerror(errno));
        }
    }
    //LOG_DBG("sendPlayers END");
}


int connectToIPMON(struct sockaddr_in* cliaddr,
#ifdef USE_MYSQL_LIB
    MYSQL* ipmon_bdd,
#endif
    char* pseudo, char* pass, ServerThreadContext* servCtx)
{
    char buf [TAILLE_BUFF];
    int connecte = 0;
    int err = ERR_NOERROR;

    Dresseur* dresseur = NULL;

    LOG_DBG("connectToIPMON: begin");

#ifdef USE_MYSQL_LIB
    dresseur = connectToIPMONSQL(ipmon_bdd, pseudo, pass);
#else
    dresseur = (Dresseur*)malloc(sizeof(Dresseur));
    if (NULL != dresseur && NULL != pseudo)
    {
        dresseur->coodX = 300;
        dresseur->coodY = 150;
        snprintf(dresseur->pseudo, MAX_SIZE_DRESSEUR_STR, "%s", pseudo);
        snprintf(dresseur->map, MAX_SIZE_DRESSEUR_STR, "tilesetIPMON.txt");
    }
    else
    {
        LOG_ERR("connectToIPMON : pseudo:%p or dresseur:%p invalid", pseudo, dresseur);
    }
#endif

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

    LOG_DBG("connectToIPMON: begin");

    return connecte;
}

int createUdpSocket(int port, int nb_max_clients) {
    LOG_DBG("createUdpSocket: begin");
    unsigned int socketServer;
    struct sockaddr_in serv_addr;

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl (INADDR_ANY);
    serv_addr.sin_port = htons (port);
    memset (&serv_addr.sin_zero, 0, sizeof(serv_addr.sin_zero));

    socketServer = socket(AF_INET, SOCK_DGRAM, 0);
    bind(socketServer, (struct sockaddr *)&serv_addr, sizeof serv_addr);
    LOG_DBG("createUdpSocket: end");

    return socketServer;
}

void processItemsInCircularBuffer(ServerThreadContext* servCtx)
{
    char* message = NULL;
    char* token = NULL;
    char pseudo[200] = "";
    char pass[200] = "";
    int code = CODE_INVALID;
    int err;

#ifdef USE_MYSQL_LIB
    MYSQL *ipmon_bdd;
    ipmon_bdd = connect_bdd();
#endif

    struct sockaddr_in cliaddr;

    LOG_DBG("processItemsInCircularBuffer: begin");

    // Main server loop for all clients
    while (1)
    {
        memset(pass, 0, 200);
        memset(pseudo, 0, 200);
        code = CODE_INVALID;
        pthread_mutex_lock(&circBufferMutex);
        err = circularBufEmpty(servCtx->circBuffer);
        pthread_mutex_unlock(&circBufferMutex);
        if (ERR_NOERROR == err)
        {
            pthread_mutex_lock(&circBufferMutex);
            circularBufRead(servCtx->circBuffer, &message, &cliaddr);
            pthread_mutex_unlock(&circBufferMutex);

            LOG_DBG("processItemsInCircularBuffer: recv message (%s)", message);

            token = strsep(&message, ":");

            if (token != NULL && message != NULL)
            {
                code = strtol(token, NULL, 10);
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
#ifdef USE_MYSQL_LIB
                        if (connectToIPMON(&cliaddr, ipmon_bdd, pseudo, pass, servCtx))
#else
                        if (connectToIPMON(&cliaddr, pseudo, pass, servCtx))
#endif
                        {
                            // TODO change state in dresseur structure
                            LOG_DBG("Connecion sucess !");
                        }
                    }
#ifdef USE_MYSQL_LIB
                    else
                    {
                        insertDresseurSQL(ipmon_bdd, pseudo, pass);
                    }
#endif
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
                    {
                        LOG_WARN("NEW_COORDINATES incorrect step1 (%s)", message);
                        break;
                    }
                    x = strtol(token, NULL, 10);
                    token = strsep(&message, ":");
                    if (token == NULL)
                    {
                        LOG_WARN("NEW_COORDINATES incorrect step2 (%s)", message);
                        break;
                    }
                    y = strtol(token, NULL, 10);
                    newPositionOfPlayer(x, y, &cliaddr, servCtx);
                    token = strsep(&message, ":");
                    if (token == NULL)
                    {
                        LOG_WARN("NEW_COORDINATES incorrect step3 (%s)", message);
                        break;
                    }
                    sendPlayers(&cliaddr, token, servCtx);

                }
                else if (code == SEND_LIST_PLAYERS) {
                    // TODO fix second argument DO NOTHING
                    LOG_WARN("Old API, use NEW_COORDINATES to receive list of players");
                    //sendPlayers(&cliaddr, strsep(&message, ":"), servCtx);
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
        else
        {
            // circular buffer is empty
            usleep(10000); // 10ms
        }

        // CheckConnection
        // TODO delete connection with no message since 3s

        usleep(2000); // 2ms
    }

    LOG_DBG("processItemsInCircularBuffer: end");
}

void *threadDownload(void *data)
{
    ServerThreadContext* servCtx = (ServerThreadContext*) data;

    struct sockaddr_in cliaddr;

    unsigned int len, n;
    char buffer[1024];
    //char ip[24];
    int err = ERR_NOERROR;
    int counterNoData = 0;

    len = sizeof(cliaddr);  //len is value/resuslt

    while (counterNoData < 1000)
    {
        memset(&cliaddr, 0, sizeof(cliaddr));
        n = recvfrom(servCtx->socketServer, (char *)buffer, 1024,
                MSG_WAITALL, ( struct sockaddr *) &cliaddr, &len);
        if (n > 0)
        {
            counterNoData = 0;
            buffer[n] = '\0';
            /*inet_ntop(AF_INET, &(((struct sockaddr_in*)&cliaddr)->sin_addr),
                        ip, 24);
            LOG_INFO("Client[%s] : %s", ip, buffer);*/
            pthread_mutex_lock(&circBufferMutex);
            err = circularBufPut(servCtx->circBuffer, buffer, &cliaddr);
            pthread_mutex_unlock(&circBufferMutex);
            if (err != ERR_NOERROR)
            {
                LOG_WARN("threadDownload %d if 1 = FULL", err);
            }

            memset(&cliaddr, 0, sizeof(cliaddr));
            memset(&buffer, 0, sizeof(buffer));
            //memset(&ip, 0, strlen(ip));
        }
        else
        {
            counterNoData++;
        }
    }

    pthread_exit(NULL);
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
