all:
	gcc -I SDL2/include -L SDL2/lib -m64 -o pacman pacman.c drawfunctions.c defaultfunctions.c -lmingw32 -lSDL2main -lSDL2 -lSDL2_image
