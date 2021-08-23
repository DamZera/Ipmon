CC=gcc
INC_DIR_COMMON=../Common/inc
SRC_DIR=src
SRCS=$(SRC_DIR)/*
OBJS=jeu_serv.o mysql_ipmon.o log.o

DEPS=$(INC_DIR_COMMON)/* inc/*
LIBS=-L/usr/local/mysql/lib -lmysqlclient -lpthread
CFLAGS=-c -Wall -I$(INC_DIR_COMMON) -Iinc/ $(LIBS)

all: $(OBJS)

%.o: $(SRC_DIR)/%.c $(DEPS)
	$(CC) -o $@ $< $(CFLAGS)

clean:
	rm -rf *.o
