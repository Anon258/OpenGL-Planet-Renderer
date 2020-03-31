#ifndef Cube_hpp
#define Cube_hpp

#include<iostream>
#include<cmath>

//GLEW
#define GLEW_STATIC
#include<GL/glew.h>

//GLFW
#include<GLFW/glfw3.h>

#include "Shader.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

class Cube{
    glm::vec4 positions[8];
    glm::vec4 glPoints[36];

    int pos_index;

    //Now we will build and compile our shaders
    
public:
	GLuint VBO,VAO;
	Shader *shader;


    void face(int a, int b, int c, int d);
    void colorcube();
    Cube(GLchar* vertshad, GLchar* fragshad);
};


#endif
