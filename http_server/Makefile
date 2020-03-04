all: server

server: server.o ../uart.o
	gcc -o server server.c ../uart.c

clean:
	rm -rf server *.o


