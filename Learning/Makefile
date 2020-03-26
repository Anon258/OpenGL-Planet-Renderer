CC=g++

OPENGLLIB= -framework OpenGL
GLEWLIB= -lGLEW
GLFWLIB = -lglfw
LIBS=$(OPENGLLIB) $(GLEWLIB) $(GLFWLIB)
LDFLAGS=-L/usr/local/lib 
CPPFLAGS=-I/usr/local/include -I./

BIN1=Bin/Create_Window
BIN2=Bin/Triangle
BIN3=Bin/Cube_Render
BIN4=Bin/MCBlock
BIN5=Bin/Cube_Rotate
BIN6=Bin/Sierpinski
BIN7=Bin/Sierpinski_Tetrahedron

SRCS1= Create_Window.cpp
SRCS2=Triangle.cpp Utility/Shader.cpp
SRCS3=Cube_Render.cpp Utility/Cube.cpp Utility/Shader.cpp
SRCS4=MCBlock.cpp Utility/Shader.cpp Utility/Textures.cpp
SRCS5=Cube_Rotate.cpp Utility/Cube.cpp Utility/Shader.cpp Utility/Camera.cpp
SRCS6=Sierpinski.cpp Utility/Shader.cpp Utility/Camera.cpp
SRCS7=Sierpinski_Tetrahedron.cpp Utility/Shader.cpp Utility/Camera.cpp

INCLUDES2=Utility/Shader.hpp
INCLUDES3=Utility/Shader.hpp Utility/Cube.hpp
INCLUDES4=Utility/Shader.hpp Utility/Textures.hpp
INCLUDES5=Utility/Shader.hpp Utility/Cube.hpp Utility/Camera.hpp
INCLUDES6=Utility/Shader.hpp Utility/Camera.hpp

all: $(BIN1) $(BIN2) $(BIN3) $(BIN4) $(BIN5) $(BIN6) $(BIN7)

$(BIN1): $(SRCS1)
	g++ $(CPPFLAGS) $(SRCS1) -o $(BIN1) $(LDFLAGS) $(LIBS)

$(BIN2): $(SRCS2) $(INCLUDES2)
	g++ $(CPPFLAGS) $(SRCS2) -o $(BIN2) $(LDFLAGS) $(LIBS)

$(BIN3): $(SRCS3) $(INCLUDES3)
	g++ $(CPPFLAGS) $(SRCS3) -o $(BIN3) $(LDFLAGS) $(LIBS)

$(BIN4): $(SRCS4) $(INCLUDES4)
	g++ $(CPPFLAGS) $(SRCS4) -o $(BIN4) $(LDFLAGS) $(LIBS)

$(BIN5): $(SRCS5) $(INCLUDES5)
	g++ $(CPPFLAGS) $(SRCS5) -o $(BIN5) $(LDFLAGS) $(LIBS)

$(BIN6): $(SRCS6) $(INCLUDES6)
	g++ $(CPPFLAGS) $(SRCS6) -o $(BIN6) $(LDFLAGS) $(LIBS)

$(BIN7): $(SRCS7) $(INCLUDES6)
	g++ $(CPPFLAGS) $(SRCS7) -o $(BIN7) $(LDFLAGS) $(LIBS)



