
all:
	gcc -Wall -pthread -o app main.c -L./ -lc_structs;
