CXX=g++ -g
CXXFLAGS=-c -Wall `sdl-config --cflags`

all: craft

craft: craft.o game.o texturemanager.o display.o object.o unit.o camera.o
	$(CXX) craft.o game.o texturemanager.o display.o object.o unit.o -lSDL -lSDL_image -lGL -lGLU -lGLEW -o craft

craft.o: craft.cpp game.h
	$(CXX) $(CXXFLAGS) craft.cpp

game.o: game.cpp game.h texturemanager.h object.h unit.h globals.h
	$(CXX) $(CXXFLAGS) game.cpp

texturemanager.o: texturemanager.cpp texturemanager.h
	$(CXX) $(CXXFLAGS) texturemanager.cpp

display.o: display.cpp display.h
	$(CXX) $(CXXFLAGS) display.cpp

camera.o: camera.cpp camera.h
	$(CXX) $(CXXFLAGS) camera.cpp

object.o: object.cpp object.h globals.h
	$(CXX) $(CXXFLAGS) object.cpp

unit.o: unit.cpp unit.h object.h
	$(CXX) $(CXXFLAGS) unit.cpp

clean:
	rm -rf *.o craft
