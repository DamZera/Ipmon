CC=gcc
INC_DIR_COMMON=../Common/inc
SRC_DIR=src
SRCS=$(SRC_DIR)/*
OBJS=map.o main.o jeu.o  GUI_ipmon.o

DEPS=$(INC_DIR_COMMON)/* inc/*
LIBS=`pkg-config --cflags --libs gtk+-3.0` `sdl2-config --cflags --libs` -lSDL2_image -lSDL2_ttf
CFLAGS=-c -Wall -I$(INC_DIR_COMMON) -Iinc/ $(LIBS)

all: $(OBJS)

%.o: $(SRC_DIR)/%.c $(DEPS)
	$(CC) -o $@ $< $(CFLAGS)

clean:
	rm -rf *.o
