all: jeu_serv.o mysql_ipmon.o log.o
	gcc -Wall jeu_serv.o mysql_ipmon.o log.o -o Serveur_ipm -L/usr/local/mysql/lib -lmysqlclient -lpthread 

log.o: log.c log.h mysql_ipmon.h jeu_serv.h protocol.h
	gcc -c log.c -o log.o 

mysql_ipmon.o: mysql_ipmon.c mysql_ipmon.h log.h
	gcc -c mysql_ipmon.c -o mysql_ipmon.o 

jeu_serv.o: jeu_serv.c jeu_serv.h log.h
	gcc -c jeu_serv.c -o jeu_serv.o

clean:
	rm -rf *.o