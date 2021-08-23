#include "mysql_ipmon.h"

#include "logger.h"

/*Fonction qui retourne un pointeur MYSQL qui permet de communiquer avec la BDD*/
MYSQL *connect_bdd(){
    MYSQL* mysql;
    /*Initialisation de MySQL*/
    mysql = mysql_init(NULL);
    /*Options de connexion*/
    mysql_options(mysql,MYSQL_READ_DEFAULT_GROUP,"option");

    if(mysql_real_connect(mysql,
            "localhost", //IP de la bdd
            "ipmon",      //User
            "ipmon2021",          //Pass
            "ipmon",      //Bdd
            8889,0,0)){
        LOG_INFO("Connexion to IPMON BDD ok..");
    }
    else {
        LOG_ERR("Error in connect_bdd() %s", mysql_error(mysql));
    }
    return mysql;
}


void finish_with_error(MYSQL *con){
  LOG_ERR("%s app closing", mysql_error(con));
  mysql_close(con);
  exit(1);        
}