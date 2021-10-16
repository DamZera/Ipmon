#ifndef MYSQL_H
#define MYSQL_H

#include"server_main.h" // TODO fix this include

MYSQL* connect_bdd();
void insertDresseurSQL(MYSQL* ipmon_bdd, char* pseudo, char* pass);
Dresseur* connectToIPMONSQL(MYSQL* ipmon_bdd, char* pseudo, char* pass);
void finish_with_error(MYSQL *con);
#endif