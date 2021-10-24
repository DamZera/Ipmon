#include "mysql_ipmon.h"

#include "logger.h"

/*
* OUTPUT : MSQL* to bdd of IPMON after connection
*/
MYSQL* connect_bdd()
{
    MYSQL* mysql;
    mysql = mysql_init(NULL);
    mysql_options(mysql,MYSQL_READ_DEFAULT_GROUP,"option");

    if(mysql_real_connect(mysql,
            "localhost", //IP de la bdd
            "ipmon",      //User
            "ipmon2021",  //Pass
            "ipmon",      //Bdd
            8889,0,0))
    {
        LOG_INFO("Connection to IPMON BDD ok..");
    }
    else
    {
        LOG_ERR("Error in connect_bdd() %s", mysql_error(mysql));
    }
    return mysql;
}

void insertDresseurSQL(MYSQL* ipmon_bdd, char* pseudo, char* pass)
{
    char query[150] = "";
    sprintf(query, "INSERT INTO dresseur VALUES('0', '%s','%s', '1','300','150','tilesetIPMON.txt');", pseudo, pass);
    LOG_INFO ("Insert dresseur, SQL query : %s", query);
    // TODO manage error case and send response to client
    mysql_query(ipmon_bdd, query);
}

Dresseur* connectToIPMONSQL(MYSQL* ipmon_bdd, char* pseudo, char* pass)
{
    char query[1024] = "";
    Dresseur* dresseur = (Dresseur*)malloc(sizeof(Dresseur));
    MYSQL_RES *result;
    MYSQL_ROW row;
    int numFields = 0;

    sprintf(query, "SELECT * FROM dresseur WHERE dresseur_name='%s' AND dresseur_pass='%s';", pseudo, pass);
    LOG_INFO ("Find dresseur, SQL query : %s ", query);
     
    if(mysql_query(ipmon_bdd, query)){
        finish_with_error(ipmon_bdd);
    }   

    result = mysql_store_result(ipmon_bdd);
    row = mysql_fetch_row(result);
    numFields = mysql_num_fields(result);

    // Dresseur find in BDD ipmon
    if (row)
    {

        /*
        TO DEBUG BDD
        LOG_DBG("row : ");
        for(int i = 0; i < numFields; i++)
        {
            LOG_DBG("%s ", row[i] ? row[i] : "NULL");
        }*/
        //              0               1                2              3               4                5                  6   
        // +-------------+---------------+---------------+--------------+----------------+----------------+------------------+
        // | dresseur_id | dresseur_name | dresseur_pass | dresseur_lvl | dresseur_pos_x | dresseur_pos_y | dresseur_map     |
        // +-------------+---------------+---------------+--------------+----------------+----------------+------------------+
        // |           1 | dressA        | 1234          |            1 |            300 |            150 | tilesetIPMON.txt |
        // +-------------+---------------+---------------+--------------+----------------+----------------+------------------+

        dresseur->coodX = strtol(row[4],NULL,10);
        dresseur->coodY = strtol(row[5],NULL,10);
        dresseur->pseudo = (char*)malloc(sizeof(char)*(strlen(pseudo)+1));
        dresseur->map = (char*)malloc(sizeof(char)*(strlen(row[6])+1));
        memset(dresseur->pseudo, 0, sizeof(char)*(strlen(pseudo)+1));
        memset(dresseur->map, 0, sizeof(char)*(strlen(row[6])+1));

        strcpy(dresseur->pseudo, pseudo);
        strcpy(dresseur->map, row[6]);

        return dresseur;
        
    }
    else
    {
        free(dresseur);
        dresseur = NULL;
    }

    return dresseur;
}


void finish_with_error(MYSQL *con)
{
  LOG_ERR("%s app closing", mysql_error(con));
  mysql_close(con);
  exit(1);        
}