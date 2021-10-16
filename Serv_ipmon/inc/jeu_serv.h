#ifndef JEU_SERV_H
#define JEU_SERV_H

#include "server_main.h"

#include "logger.h"

void combat_init(MYSQL* ipmon_bdd,char* msg,int *s_dial);
int combat(Ipmon ipmonDresseur, Ipmon ipmon, int *s_dial);
Ipmon* chercher_ipmon(MYSQL* ipmon_bdd,int id);
int attaque (Ipmon* ipmon1, Ipmon* ipmon2);
int attaque_speciale (Ipmon* ipmon1, Ipmon* ipmon2);
#endif