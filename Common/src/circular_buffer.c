#include "circular_buffer.h"

/*
* NAME   	: create_CircularBuf()
* INPUT 	: _capacity : capacity of circular CircularBuf
* OUTPUT 	: NULL on failure.
*		      pointer to created circular CircularBuf (CircularBuf*)
* DESCRIPTION	: determine if CircularBuf is empty
*/
CircularBuf* createCircularBuf(unsigned int _capacity){

    CircularBuf* myCircularBuf = (CircularBuf*)malloc(sizeof(CircularBuf)); // allocate memory of size of CircularBuf struct

    if (myCircularBuf == NULL ){
        return NULL;
    } else {
        // populate the variables of the CircularBuf :
        myCircularBuf->tail = myCircularBuf;
        myCircularBuf->head = myCircularBuf;
        myCircularBuf->next = NULL;
        myCircularBuf->size = 0;
        myCircularBuf->capacity = _capacity;
        myCircularBuf->data = NULL;

        CircularBuf* prevCircularBuf = myCircularBuf;
        CircularBuf* currCircularBuf = NULL;
        for(int i = 0; i < _capacity; i++) {
            currCircularBuf = (CircularBuf*)malloc(sizeof(CircularBuf));
            currCircularBuf->tail = NULL;
            currCircularBuf->head = NULL;
            currCircularBuf->next = NULL;
            currCircularBuf->size = 0;
            currCircularBuf->capacity = _capacity;
            currCircularBuf->data = NULL;

            prevCircularBuf->next = currCircularBuf;
            prevCircularBuf = currCircularBuf;
        }

        prevCircularBuf->next = myCircularBuf->next;

        return myCircularBuf;
    }
}

/*
* NAME 	: CircularBuf_empty()
* INPUT 	: q : pointer to circular CircularBuf (CircularBuf*).
* OUTPUT 	: ERR_PARAMETER if q is NULL
* 		  CIRCULAR_BUFF_EMPTY if q is empty
* 		  ERR_NOERROR if q is not empty
* DESCRIPTION	: determine if CircularBuf is empty
*/
int circularBufEmpty(CircularBuf* q){
        if (q == NULL){
            return ERR_PARAMETER;
        }else if(q->size == 0) {
            return CIRCULAR_BUFF_EMPTY;
        }else {
            return ERR_NOERROR;
        }
}

/*
* NAME 	    : CircularBuf_full()
* INPUT 	: q : pointer to circular CircularBuf (CircularBuf*).
* OUTPUT 	: ERR_PARAMETER if q is NULL
* 		  CIRCULAR_BUFF_FULL if q is full
* 		  ERR_NOERROR if q is not full
* DESCRIPTION	: determine if CircularBuf is full
*/
int circularBufFull(CircularBuf* q){
    if (q == NULL){
        return ERR_PARAMETER;
    }else if(q->size == q->capacity){
        return CIRCULAR_BUFF_FULL;
    }else{
        return ERR_NOERROR;
    }
}

// NAME 	:	CircularBuf_enCircularBuf()
// INPUT 	: q : pointer to circular CircularBuf (CircularBuf*)
// 		  item : integer to be added to CircularBuf
// OUTPUT 	: ERR_PARAMETER if q is NULL
// 		  ERR_NOERROR if item was added successfully
// 		  CIRCULAR_BUFF_FULL otherwise
// DESCRIPTION	: EnCircularBuf item into circular CircularBuf q.
int circularBufPut(CircularBuf* q, char* item, struct sockaddr_in* cliaddr){

        if (q == NULL){
            return ERR_PARAMETER;
        }	else if (circularBufFull(q) == CIRCULAR_BUFF_FULL){
            return CIRCULAR_BUFF_FULL;
        } else {
            q->tail->data = (char*)malloc(sizeof(char)*(strlen(item)+1));
            memcpy(q->tail->data, item, strlen(item)+1);

            q->tail->cliaddr.sin_family = AF_INET;
            q->tail->cliaddr.sin_port = cliaddr->sin_port;
            q->tail->cliaddr.sin_addr.s_addr = cliaddr->sin_addr.s_addr;

            q->tail = q->tail->next;
            // now we have to increase the size.
            q->size++;
            return ERR_NOERROR;
        }
}

// NAME 	: CircularBuf_enCircularBuf()
// INPUT 	: q : pointer to circular CircularBuf (CircularBuf*)
//            item will be set with read value
// OUTPUT 	: ERR_PARAMETER if q is NULL
// 		  0 if q is Empty
// DESCRIPTION	: DeCircularBuf circular CircularBuf q, returning next value.
// Note : we are ASSUMING all values in q are greater than zero.
int circularBufRead(CircularBuf *q, char** item, struct sockaddr_in* cliaddr){

        if (q == NULL)
        {
            return ERR_PARAMETER;

        }
        else if (circularBufEmpty(q) == CIRCULAR_BUFF_EMPTY)
        {
            return CIRCULAR_BUFF_FULL;
        }
        else
        {
            *item = (char*)malloc(sizeof(char)*(strlen(q->head->data)+1));
            memcpy((char*)(*item), q->head->data, strlen(q->head->data)+1);
            free(q->head->data);

            //memcpy(cliaddr, q->head->cliaddr, sizeof(q->head->cliaddr));
            cliaddr->sin_family = AF_INET;
            cliaddr->sin_port = q->head->cliaddr.sin_port;
            cliaddr->sin_addr.s_addr = q->head->cliaddr.sin_addr.s_addr;

            q->head->data = NULL;
            q->head = q->head->next;
            q->size--;

            return ERR_NOERROR;
        }
    }

// NAME 	: CircularBuf_size()
// INPUT 	: q : pointer to circular CircularBuf (CircularBuf*).
// OUTPUT 	: ERR_PARAMETER if q is NULL
// 		  else return current size of circular CircularBuf q.
// DESCRIPTION	: determine size of CircularBuf q.
unsigned int circularBufSize(CircularBuf* q){
    if (q == NULL){
        return ERR_PARAMETER;
    } else {
        return q->size;
    }
}

// NAME 	: free_CircularBuf()
// INPUT 	: q : pointer to circular CircularBuf (CircularBuf*).
// OUTPUT 	: NONE
// DESCRIPTION	: free memory associatioed with circular CircularBuf q
void freeCircularBuf(CircularBuf* q){
    // free the array
    free(q->data);
    // free CircularBuf
    free(q);
}


/* EXAMPLE
int main(void) {
    CircularBuf* cBuf = createCircularBuf(6);
    char buf[10];

    for (int i = 0; i < 6; i++) {
        sprintf(buf, "%d", i);
        int err = circularBufPut(cBuf, buf);
        if (ERR_NOERROR != err) {
            printf("cBuf is full and can not add %s\n", buf);
        }
    }

    char* item = NULL;
    printf("cBuf size %i\n", circularBufSize(cBuf));
    circularBufRead(cBuf, &item);
    printf("cBuf read %s\n", item); free(item);
    circularBufRead(cBuf, &item);
    printf("cBuf read %s\n", item); free(item);
    circularBufRead(cBuf, &item);
    printf("cBuf read %s\n", item); free(item);
    circularBufRead(cBuf, &item);
    printf("cBuf read %s\n", item); free(item);
    circularBufRead(cBuf, &item);
    printf("cBuf read %s\n", item); free(item);
    circularBufRead(cBuf, &item);
    printf("cBuf read %s\n", item); free(item);
    printf("cBuf size %i\n", circularBufSize(cBuf));
}*/