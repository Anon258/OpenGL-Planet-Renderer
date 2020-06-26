#ifndef Atmosphere_hpp
#define Atmosphere_hpp

#include<iostream>
#include<GL/glew.h>

#include "Shader.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

class Atmosphere{
public:
    GLuint atmVAO,atmVBO;
    Shader* atmShader;
    Atmosphere();
    void renderAtm(glm::mat4 view, glm::mat4 projection, glm::vec3 lightDirection, glm::vec3 cent, float fov);
};

#endif /* Atmosphere_hpp */
