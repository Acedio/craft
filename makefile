CXX=g++ -g
CXXFLAGS=-c -Wall `sdl-config --cflags`

OBJ=craft.o game.o texturemanager.o display.o input.o gridmap.o modelmanager.o objectmanager.o object.o unit.o unit_worker.o camera.o soundmanager.o resource.o resource_tree.o resource_gold.o

all: craft

craft: $(OBJ)
	$(CXX) $(OBJ) -lSDL -lSDL_image -lSDL_mixer -lGL -lGLU -lGLEW -o craft

craft.o: craft.cpp game.h

game.o: game.cpp game.h texturemanager.h object.h unit.h globals.h modelmanager.h

texturemanager.o: texturemanager.cpp texturemanager.h

input.o: input.cpp input.h

display.o: display.cpp display.h

camera.o: camera.cpp camera.h

gridmap.o: gridmap.cpp gridmap.h globals.h

modelmanager.o: modelmanager.cpp modelmanager.h

objectmanager.o: objectmanager.cpp objectmanager.h object.h

object.o: object.cpp object.h globals.h modelmanager.h texturemanager.h

unit.o: unit.cpp unit.h modelmanager.h texturemanager.h

unit_worker.o: unit_worker.cpp unit_worker.h modelmanager.h texturemanager.h

soundmanager.o: soundmanager.cpp soundmanager.h

resource.o: resource.cpp resource.h modelmanager.h texturemanager.h

resource_tree.o: resource_tree.cpp resource_tree.h modelmanager.h texturemanager.h

resource_gold.o: resource_gold.cpp resource_gold.h modelmanager.h texturemanager.h

clean:
	rm -rf *.o craft
