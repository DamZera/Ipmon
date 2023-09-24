#ifndef LOG_H
#define LOG_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <pthread.h>

#include <malloc.h>

#ifdef USE_MYSQL_LIB
#include <mysql/mysql.h>
#endif
#include "protocol.h"
#include "circular_buffer.h"

#define MAX_CLIENTS 5
#define MAX_SIZE_IPMON_STR 100

#define MAX_SIZE_DRESSEUR_STR 200

#define TIMEOUT_PLAYER 3

typedef struct Ipmon {
    int id;
    char nom[MAX_SIZE_IPMON_STR];
    char etat[MAX_SIZE_IPMON_STR];
    char type[MAX_SIZE_IPMON_STR];
    int typeEntier; //Valeur du type d'ipmon : intervient dans le calcul du succès ou non d'une attaque
    int pv; //Point de vie
    int agilite; //Rapidité de reaction d'un ipmon
    int niveau; //Niveau de l'ipmon : intervient dans le calcul du degat subit par un ipmon [1 - 100]
    int puissance_attaque; //Puissance d'attaque simple
    char nom_attaque[MAX_SIZE_IPMON_STR];
    int precision_attaque;//Précision de l'attaque simple
    int puissance_defense; //Puissance de defense : intervient dans le calcul du degat subit par un ipmon
    char nom_defense[MAX_SIZE_IPMON_STR];
    int esquive; //Capacité d'esquive d'un ipmon : intervient dans le calcul du succès ou non d'une attaque = 100
    int precision; //Précision de l'ipmon
    int puissance_attaque_special;//Puissance de l'attaque spéciale d'un ipmon
    int precision_attaque_special;//Précision de l'attaque spéciale d'un ipmon
    char nom_attaque_special[MAX_SIZE_IPMON_STR];
    int puissance_defense_special;//Puissance de la défense spéciale d'un ipmon : intervient lorsqu'un ipmon est attaqué par une attaque spéciale
    char nom_defense_special[MAX_SIZE_IPMON_STR];
}Ipmon;

typedef struct Dresseur{
    int id;
    char pseudo[MAX_SIZE_DRESSEUR_STR];
    char map[MAX_SIZE_DRESSEUR_STR];
    int niveau;
    int coodX;
    int coodY;
    unsigned long timeOfLastMessage; // (unsigned long)time(NULL)
    struct sockaddr_in cliaddr;
    struct Dresseur* next;
    Ipmon* ipmons;
}Dresseur;

typedef struct {
    int socketServer;
    CircularBuf* circBuffer;
    Dresseur *dresseursList;
} ServerThreadContext;

#ifdef USE_MYSQL_LIB
int connectToIPMON(struct sockaddr_in* cliaddr, MYSQL* ipmon_bdd, char* pseudo, char* pass, ServerThreadContext* servCtx);
#else
int connectToIPMON(struct sockaddr_in* cliaddr, char* pseudo, char* pass, ServerThreadContext* servCtx);
#endif
void afficher_dresseur(Dresseur *dresseur_list);
int next_client_num(Dresseur **dresseurs);
void remove_dresseur(int socket);
Dresseur* addPlayer(Dresseur *dresseur_list, struct sockaddr_in* cliaddr, char* pseudo, int coodX, int coodY, char* map, int id);
Dresseur* chercher_dresseur(Dresseur *dresseur_list, int socket);
Dresseur* searchDresseurPseudo(Dresseur *dresseur_list, char* pseudo);
void newPositionOfPlayer(int coodX, int coodY, struct sockaddr_in* cliaddr, ServerThreadContext* servCtx);
void sendPlayers(struct sockaddr_in* cliaddr,char *map, ServerThreadContext* servCtx);
void checkConnectionOfPlayers(ServerThreadContext* servCtx); // Delete players from list not connected since 3s

//void envoieIpmonDresseur(int s_dial,MYSQL* ipmon_bdd, Dresseur* dresseur_list);

void processItemsInCircularBuffer(ServerThreadContext* servCtx);
void *threadDownload(void *data);

#endif
