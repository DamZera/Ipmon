#ifndef GUI_IPMON_H
#define GUI_IPMON_H

#include "logger.h"

#include "main.h"
#include "map.h"

#include <unistd.h>
#include <time.h>
#include <sys/time.h>

#define SGN(X) (((X)==0)?(0):(((X)<0)?(-1):(1)))
#define ABS2(X) ((((X)<0)?(-(X)):(X)))

#define LARGEUR_FENETRE 600
#define HAUTEUR_FENETRE 600
#define MOVE_SPEED_PLAYER 5

#endif