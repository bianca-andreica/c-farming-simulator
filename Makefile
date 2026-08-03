
all:
	gcc -Wall -std=c99 -Iinclude -c src/game.c -o src/game.o
	gcc -Wall -std=c99 -Iinclude -c src/render.c -o src/render.o
	gcc -Wall -std=c99 -Iinclude -c src/main.c -o src/main.o
	gcc -o farming_game.exe src/main.o src/game.o src/render.o -Llib -lraylib -lopengl32 -lgdi32 -lwinmm

clean:
	del /Q src/*.o farming_game.exe

