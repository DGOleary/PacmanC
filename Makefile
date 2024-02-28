CC = gcc
CFLAGS = -I SDL2/include -L SDL2/lib -m64
LIBS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_image
OTHERS = drawfunctions.c defaultfunctions.c

all: program test

program: pacman.c $(OTHERS)
	$(CC) $(CFLAGS) -o pacman pacman.c $(OTHERS) $(LIBS)

test: test.c $(OTHERS)
	$(CC) $(CFLAGS) -o test test.c $(OTHERS) $(LIBS)

clean:
	rm -f pacman test
