CC=gcc
CFLAGS=`pkg-config fuse3 --cflags` -Wall
LIBS=`pkg-config fuse3 --libs`

all:
	$(CC) main.c cow.c -o mini_unionfs $(CFLAGS) $(LIBS)
