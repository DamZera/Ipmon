#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <netinet/in.h>

#define CIRCULAR_BUFF_FULL 0x1
#define CIRCULAR_BUFF_EMPTY 0x2

#define ERR_PARAMETER 0x3
#define ERR_NOERROR 0x0

typedef struct CircularBuf CircularBuf;

struct CircularBuf{
    CircularBuf* head;
    CircularBuf* tail;
    CircularBuf* next;
    unsigned int size;
    unsigned int capacity;
    struct sockaddr_in cliaddr;
    char* data;
};

/*
* NAME   	: create_CircularBuf()
* INPUT 	: _capacity : capacity of circular CircularBuf
* OUTPUT 	: NULL on failure.
*		      pointer to created circular CircularBuf (CircularBuf*)
* DESCRIPTION	: determine if CircularBuf is empty
*/
CircularBuf* createCircularBuf(unsigned int _capacity);

/*
* NAME 	: CircularBuf_empty()
* INPUT 	: q : pointer to circular CircularBuf (CircularBuf*).
* OUTPUT 	: ERR_PARAMETER if q is NULL
* 		  CIRCULAR_BUFF_EMPTY if q is empty
* 		  ERR_NOERROR if q is not empty
* DESCRIPTION	: determine if CircularBuf is empty
*/
int circularBufEmpty(CircularBuf* q);

/*
* NAME 	    : CircularBuf_full()
* INPUT 	: q : pointer to circular CircularBuf (CircularBuf*).
* OUTPUT 	: ERR_PARAMETER if q is NULL
* 		  CIRCULAR_BUFF_FULL if q is full
* 		  ERR_NOERROR if q is not full
* DESCRIPTION	: determine if CircularBuf is full
*/
int circularBufFull(CircularBuf* q);

// NAME 	:	CircularBuf_enCircularBuf()
// INPUT 	: q : pointer to circular CircularBuf (CircularBuf*)
// 		  item : integer to be added to CircularBuf
// OUTPUT 	: ERR_PARAMETER if q is NULL
// 		  ERR_NOERROR if item was added successfully
// 		  CIRCULAR_BUFF_FULL otherwise
// DESCRIPTION	: EnCircularBuf item into circular CircularBuf q.
int circularBufPut(CircularBuf* q, char* item, struct sockaddr_in* cliaddr);

// NAME 	: CircularBuf_enCircularBuf()
// INPUT 	: q : pointer to circular CircularBuf (CircularBuf*)
//            item will be set with read value
// OUTPUT 	: ERR_PARAMETER if q is NULL
// 		  0 if q is Empty
// DESCRIPTION	: DeCircularBuf circular CircularBuf q, returning next value.
// Note : we are ASSUMING all values in q are greater than zero.
int circularBufRead(CircularBuf *q, char** item, struct sockaddr_in* cliaddr);

// NAME 	: CircularBuf_size()
// INPUT 	: q : pointer to circular CircularBuf (CircularBuf*).
// OUTPUT 	: ERR_PARAMETER if q is NULL
// 		  else return current size of circular CircularBuf q.
// DESCRIPTION	: determine size of CircularBuf q.
unsigned int circularBufSize(CircularBuf* q);

// NAME 	: free_CircularBuf()
// INPUT 	: q : pointer to circular CircularBuf (CircularBuf*).
// OUTPUT 	: NONE
// DESCRIPTION	: free memory associatioed with circular CircularBuf q
void freeCircularBuf(CircularBuf* q);