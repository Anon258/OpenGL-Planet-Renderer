#include "SkyBox.hpp"

SkyBox::SkyBox(){
    //Generate CubeMap
    skyboxShader = new Shader("Shaders/sky_vshad.glsl","Shaders/sky_fshad.glsl");
    skyboxShader->use();
    skyboxShader->setUniformInt("skybox", 0);
    std::vector<std::string> faces{
        "Images/space/milkyway2.jpg",   //right
        "Images/space/stars1.jpg",      //left
        "Images/space/stars1.jpg",      //top
        "Images/space/stars2.jpg",      //bottom
        "Images/space/milkyway1.jpg",   //front
        "Images/space/stars2.jpg"       //back
    };
    cubemapTexture = loadCubemap(faces);
    glGenVertexArrays(1, &skyVAO);
    glGenBuffers(1, &skyVBO);
    
    glBindVertexArray(skyVAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, skyVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);
    
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
}

GLuint SkyBox::loadCubemap(std::vector<std::string> faces)
{
    GLuint textureID;
    glGenTextures(1, &textureID);
    stbi_set_flip_vertically_on_load(false);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
    
    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            std::cout<<faces[i]<<std::endl;
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    
    return textureID;
}

int SkyBox::vertCount(){
    return sizeof(skyboxVertices)/(sizeof(GLfloat)*3);
}

void SkyBox::render(glm::mat4 view, glm::mat4 projection){
    glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
    view = glm::mat4(glm::mat3(view)); // remove translation from the view matrix to give feeling of infiniteness to skybox
    skyboxShader->use();
    skyboxShader->setUniformMat4("view", view);
    skyboxShader->setUniformMat4("projection", projection);
    // skybox cube
    glBindVertexArray(skyVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glDepthFunc(GL_LESS); // set depth function back to default
}
