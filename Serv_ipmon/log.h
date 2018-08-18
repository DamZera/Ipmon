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

#include <sys/malloc.h>
#include <mysql/mysql.h>
//DEFINE
#define MAX_CLIENTS 5 


//gcc -o log log.c -libmysqlclient
typedef struct Ipmon {
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
}Ipmon;

typedef struct Dresseur{
	int id;
	char* pseudo;
	char* map;
	int niveau;
	int sock;
	int coodX;
	int coodY;
	struct Dresseur* next;
	Ipmon* ipmons;
}Dresseur;

typedef struct Message{
	char code [50];
	char data [50];
}Message;


char *str_sub (const char *s, unsigned int start, unsigned int end);

int query_ipmonbdd(Message *msg, int *s_dial);
void insert_dresseur(int *s_dial, MYSQL* ipmon_bdd);
int connection_dresseur(int *s_dial, MYSQL* ipmon_bdd);
void afficher_dresseur(Dresseur *dresseur_list);
int next_client_num(Dresseur **dresseurs);
void remove_dresseur(int socket);
Dresseur* ajouter_dresseur(Dresseur *dresseur_list, int socket, char* pseudo, int coodX, int coodY, char* map, int id);
Dresseur* chercher_dresseur(Dresseur *dresseur_list,int socket);

void envoieIpmonDresseur(int s_dial,MYSQL* ipmon_bdd, Dresseur* dresseur_list);

#endif