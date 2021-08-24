#ifndef MYSQL_H
#define MYSQL_H

#include"log.h"

MYSQL* connect_bdd();
void finish_with_error(MYSQL *con);
#endif