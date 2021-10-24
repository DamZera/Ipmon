#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>

#include <malloc.h>

#include "protocol.h"
#include "logger.h"

typedef struct Login{
    struct sockaddr_in* srvaddr;
    char pseudo[50];
    char pass[50]; 
    int socket;
    int connect;
}Login;

typedef struct Ipmon{
    int id;
    char* nom;
    char* etat;
    char* type;
    int typeEntier; //Valeur du type d'ipmon : intervient dans le calcul du succès ou non d'une attaque
    int pv; //Point de vie
    int agilite; //Rapidité de reaction d'un ipmon
    int niveau; //Niveau de l'ipmon : intervient dans le calcul du degat subit par un ipmon [1 - 100]
    int puissance_attaque; //Puissance d'attaque simple
    char* nom_attaque;
    int precision_attaque;//Précision de l'attaque simple
    int puissance_defense; //Puissance de defense : intervient dans le calcul du degat subit par un ipmon
    char* nom_defense;
    int esquive; //Capacité d'esquive d'un ipmon : intervient dans le calcul du succès ou non d'une attaque = 100
    int precision; //Précision de l'ipmon
    int puissance_attaque_special;//Puissance de l'attaque spéciale d'un ipmon
    int precision_attaque_special;//Précision de l'attaque spéciale d'un ipmon
    char* nom_attaque_special;
    int puissance_defense_special;//Puissance de la défense spéciale d'un ipmon : intervient lorsqu'un ipmon est attaqué par une attaque spéciale
    char* nom_defense_special;
    struct Ipmon* next;
}Ipmon;

typedef struct Dresseur{
    char* id;
    char pseudo[100];
    char map[100];
    int niveau;
    int sock;
    int coodX;
    int coodY;
    struct Dresseur* next;
    Ipmon *ipmons;
    int nombreIpmons;
}Dresseur;

/******************************
 *  Command line functions
 *****************************/

/**
 * @brief Print command manual
 * 
 * @param char* cmd : command name
 **/ 
void printManual(char* cmd);

/**
 * @brief Process command enter by the user
 * 
 * @param int socket : socket open with server
 * @param struct sockaddr_in* srvaddr : socket address to communicate with the server
 * @param char* cmd : command to process
 * 
 * @return int : connection with the server is ok or not
 *             1 -> OK
 *             0 -> KO
 **/ 
int processCommand(int socket, struct sockaddr_in* srvaddr, char* cmd);

/*********************************************
 *  IPMON connection and register of player
 ********************************************/

/**
 * @brief Create socket to send / receive message
 * 
 * @param int port : port choose for the client
 * 
 * @return int socket : -1 when the creation of the socket fail
 **/
int createSocket(int port);

/**
 * @brief Register player with the data from login struct
 * 
 * @param Login* login : pointer to the login structure
 **/
void registerPlayer(Login* login);

/**
 * @brief Connect player with the data from login struct
 * 
 * @param Login* login : pointer to the login structure
 **/
int connectPlayerToIPMON(Login* login);

#endif 
