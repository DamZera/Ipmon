#ifndef PROTOCOL_H
#define PROTOCOL_H

typedef enum 
{
	CONNECTION          = 0,
	REGISTER            = 2,
	NEW_COORDINATES     = 4,
	COMBAT              = 6,
	CLOSE				= 8
} client_code_msg;

typedef enum 
{
    ACCEPT              = 1,
    REFUSE              = 3,
    ACCEPT_REGISTER     = 5
} server_code_msg;

#endif