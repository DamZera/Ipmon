#ifndef JEU_SERV_H
#define JEU_SERV_H

#include "log.h"

#include "logger.h"

int jeu(int *s_dial, MYSQL* ipmon_bdd, Dresseur *dresseur_list, pthread_mutex_t* mutex_client);
void deplacement(char* data, int socket, pthread_mutex_t* mutex_client);
void nouvelle_coord_dresseur(int coodX, int coodY, int socket);
void envoieDresseurs(int *s_dial,char *map);
void combat_init(MYSQL* ipmon_bdd,char* msg,int *s_dial);
int combat(Ipmon ipmonDresseur, Ipmon ipmon, int *s_dial);
Ipmon* chercher_ipmon(MYSQL* ipmon_bdd,int id);
int attaque (Ipmon* ipmon1, Ipmon* ipmon2);
int attaque_speciale (Ipmon* ipmon1, Ipmon* ipmon2);
#endif