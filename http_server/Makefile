all: server

server: server.o ../uart.o
	gcc -g -o server server.c ../uart.c

clean:
	rm -rf server *.o


