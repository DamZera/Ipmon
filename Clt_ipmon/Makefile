all:  map.o event.o main.o jeu.o  GUI_ipmon.o
	gcc -Wall -o main main.o map.o event.o jeu.o GUI_ipmon.o `pkg-config --cflags --libs gtk+-2.0` `sdl-config --cflags --libs` -lSDL_ttf

GUI_ipmon.o: GUI_ipmon.c event.h map.h jeu.h
	gcc -c `pkg-config --cflags --libs gtk+-2.0` GUI_ipmon.c -o GUI_ipmon.o 

event.o: event.c event.h
	gcc -c `pkg-config --cflags --libs gtk+-2.0` event.c -o event.o

main.o: main.c main.h protocol.h
	gcc -c `pkg-config --cflags --libs gtk+-2.0` main.c -o main.o 

map.o: map.c map.h
	gcc -c `pkg-config --cflags --libs gtk+-2.0` map.c -o map.o

jeu.o: jeu.c jeu.h
	gcc -c `pkg-config --cflags --libs gtk+-2.0` jeu.c -o jeu.o

clean:
	rm -rf *.o
