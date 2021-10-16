CC=gcc
INC_DIR_COMMON=../Common/inc
SRC_DIR=src
SRCS=$(SRC_DIR)/* ../Common/$(SRC_DIR)/*
OBJS=jeu_serv.o mysql_ipmon.o server_main.o circular_buffer.o

DEPS=$(INC_DIR_COMMON)/* inc/*
LIBS=-L/usr/local/mysql/lib -lmysqlclient -lpthread
CFLAGS=-Wall -I$(INC_DIR_COMMON) -Iinc/ $(LIBS)

all: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o serv $(CFLAGS)

%.o: $(SRC_DIR)/%.c $(DEPS)
	$(CC) -o $@ -c $< $(CFLAGS)

%.o: ../Common/$(SRC_DIR)/* $(DEPS)
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -rf *.o
