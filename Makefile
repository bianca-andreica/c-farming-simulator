CC = gcc
CFLAGS = -Wall -std=c99 -Iinclude

SRC = src/main.c src/game.c src/render.c
OBJ = src/main.o src/game.o src/render.o

TARGET = farming_game

UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Linux)
    LDLIBS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
else
    LDLIBS = -Llib -lraylib -lopengl32 -lgdi32 -lwinmm
endif

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) -o $@ $(OBJ) $(LDLIBS)

src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f src/*.o farming_game farming_game.exe
