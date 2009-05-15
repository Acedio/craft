CXX=g++ -g
CXXFLAGS=-c -Wall `sdl-config --cflags`

OBJ=craft.o game.o texturemanager.o display.o input.o gridmap.o modelmanager.o objectmanager.o object.o unit.o $(patsubst unit_%.cpp,unit_%.o,$(wildcard unit_*.cpp)) camera.o soundmanager.o resource.o $(patsubst resource_%.cpp,resource_%.o,$(wildcard resource_*.cpp)) building.o $(patsubst building_%.cpp,building_%.o,$(wildcard building_*.cpp)) globals.o 
all: craft

craft: $(OBJ)
	$(CXX) $(OBJ) -lSDL -lSDL_image -lSDL_mixer -lGL -lGLU -lGLEW -o craft

globals.o: globals.cpp globals.h

craft.o: craft.cpp game.h globals.h

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

unit_%.o: unit_%.cpp unit_%.h modelmanager.h texturemanager.h

building.o: building.cpp building.h modelmanager.h texturemanager.h

building_%.o: building_%.cpp building_%.h modelmanager.h texturemanager.h

soundmanager.o: soundmanager.cpp soundmanager.h

resource.o: resource.cpp resource.h modelmanager.h texturemanager.h

resource_%.o: resource_%.cpp resource_%.h modelmanager.h texturemanager.h

clean:
	rm -rf *.o craft
