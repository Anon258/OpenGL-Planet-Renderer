CC=g++

OPENGLLIB= -framework OpenGL
GLEWLIB= -lGLEW
GLFWLIB = -lglfw
LIBS=$(OPENGLLIB) $(GLEWLIB) $(GLFWLIB)
LDFLAGS=-L/usr/local/lib 
CPPFLAGS=-I/usr/local/include -I./

BIN1=Bin/Create_Window
BIN2=Bin/Triangle
BIN3=Bin/Cube
BIN4=Bin/MCBlock
SRCS1= Create_Window.cpp
SRCS2=Triangle.cpp Utility/Shader.cpp
SRCS3=Cube.cpp Utility/Shader.cpp
SRCS4=MCBlock.cpp Utility/Shader.cpp Utility/Textures.cpp
INCLUDES2=Utility/Shader.hpp
INCLUDES3=Utility/Shader.hpp Utility/Cube.hpp
INCLUDES4=Utility/Shader.hpp Utility/Textures.hpp

all: $(BIN1) $(BIN2) $(BIN3) $(BIN4)

$(BIN1): $(SRCS1)
	g++ $(CPPFLAGS) $(SRCS1) -o $(BIN1) $(LDFLAGS) $(LIBS)

$(BIN2): $(SRCS2) $(INCLUDES2)
	g++ $(CPPFLAGS) $(SRCS2) -o $(BIN2) $(LDFLAGS) $(LIBS)

$(BIN3): $(SRCS3) $(INCLUDES3)
	g++ $(CPPFLAGS) $(SRCS3) -o $(BIN3) $(LDFLAGS) $(LIBS)

$(BIN4): $(SRCS4) $(INCLUDES4)
	g++ $(CPPFLAGS) $(SRCS4) -o $(BIN4) $(LDFLAGS) $(LIBS)



