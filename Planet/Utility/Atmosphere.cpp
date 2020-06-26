#include "Atmosphere.hpp"

Atmosphere::Atmosphere(){
    atmShader = new Shader("Shaders/scatter_vert.glsl", "Shaders/scatter_frag.glsl");
    
    glm::vec3 scatter_quad[] = {
        glm::vec3(-1.0,-1.0,-1.0),
        glm::vec3(-1.0, 1.0,-1.0),
        glm::vec3( 1.0,-1.0,-1.0),
        glm::vec3( 1.0, 1.0,-1.0),
        glm::vec3(-1.0, 1.0,-1.0),
        glm::vec3( 1.0,-1.0,-1.0)
    };
    
    glGenVertexArrays(1, &atmVAO);
    glGenBuffers(1, &atmVBO);
    glBindVertexArray(atmVAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, atmVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(scatter_quad), scatter_quad, GL_STATIC_DRAW);
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    atmShader->use();
    atmShader->setUniformFloat("earth_radius", 10.0f);
    atmShader->setUniformFloat("atm_radius", 10.0 * 67.0f/64.0f);
}

void Atmosphere::renderAtm(glm::mat4 view, glm::mat4 projection, glm::vec3 lightDirection, glm::vec3 cent, float fov){
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    atmShader->use();
    glm::vec3 center = glm::vec3(view * glm::vec4(0,0,0,1.0));
    atmShader->setUniformVec3("earth_center", center);
    glm::vec3 lightdir = glm::vec3(view * glm::vec4(lightDirection, 0.0));
    lightdir = glm::normalize(lightdir);
    atmShader->setUniformVec3("light_direction", lightdir);
    atmShader->setUniformFloat("fov", fov);
    
    glBindVertexArray(atmVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
    glBlendFunc(GL_ONE, GL_ZERO);
}
