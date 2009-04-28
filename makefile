TARGET = craft
SRCS = craft.cpp display.cpp game.cpp gridmap.cpp object.cpp objectmanager.cpp texturemanager.cpp unit.cpp
OBJS = $(SRCS:.c=.o)
INCLUDES = 
LIBS = -lSDL -lSDL_image -lGL -lGLU -lglew
LDFLAGS = $(LIBS)
CXX = g++
CXXFLAGS = -g -Wall

$(TARGET): $(INCLUDES) $(OBJS) $(LIBS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS)

clean:
	rm -f $(OBJS) $(TARGET)
