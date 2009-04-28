CC=g++ -g
CFLAGS=-c -Wall `sdl-config --cflags`

all: craft

craft: craft.o game.o texturemanager.o object.o unit.o
	$(CC) craft.o game.o texturemanager.o object.o unit.o -lSDL -lSDL_image -lGL -lGLU -lGLEW -o craft

craft.o: craft.cpp game.h
	$(CC) $(CFLAGS) craft.cpp

game.o: game.cpp game.h texturemanager.h object.h unit.h globals.h
	$(CC) $(CFLAGS) game.cpp

texturemanager.o: texturemanager.cpp texturemanager.h
	$(CC) $(CFLAGS) texturemanager.cpp

object.o: object.cpp object.h globals.h
	$(CC) $(CFLAGS) object.cpp

unit.o: unit.cpp unit.h object.h
	$(CC) $(CFLAGS) unit.cpp

clean:
	rm -rf *.o craft
