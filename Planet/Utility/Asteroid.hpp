#ifndef Asteroid_hpp
#define Asteroid_hpp

#include<string>
#include<iostream>
#include<vector>
#include "stb_image.h"
#include<GL/glew.h>
#include<GLFW/glfw3.h>

#include "Shader.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Model.hpp"

class Asteroid{
public:
    unsigned int amount;
    glm::mat4 *modelMatrices;
    GLuint buffer;
    Model *rock;
    Shader *asteroidShader;
    
    void InitBuffers();
    Asteroid();
    void render(glm::mat4 view, glm::mat4 projection);
};
#endif
