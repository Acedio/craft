CXX=g++ -g
CXXFLAGS=-c -Wall `sdl-config --cflags`

OBJ=craft.o game.o texturemanager.o display.o input.o modelmanager.o objectmanager.o object.o unit.o camera.o

all: craft

craft: $(OBJ)
	$(CXX) $(OBJ) -lSDL -lSDL_image -lGL -lGLU -lGLEW -o craft

craft.o: craft.cpp game.h

game.o: game.cpp game.h texturemanager.h object.h unit.h globals.h modelmanager.h

texturemanager.o: texturemanager.cpp texturemanager.h

input.o: input.cpp input.h

display.o: display.cpp display.h

camera.o: camera.cpp camera.h

modelmanager.o: modelmanager.cpp modelmanager.h

objectmanager.o: objectmanager.cpp objectmanager.h object.h

object.o: object.cpp object.h globals.h

unit.o: unit.cpp unit.h object.h

clean:
	rm -rf *.o craft
