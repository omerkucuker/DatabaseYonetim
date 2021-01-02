CC=gcc
CFLAGS=-c -Wall

all: program database kaydet

program: program.c
	$(CC) program.c -o program	

database: database.c
	$(CC) database.c -o database
	
kaydet:	kaydet.c
	$(CC) kaydet.c -o kaydet

clean:
	rm -rf *o main