#include "Asteroid.hpp"

void Asteroid::InitBuffers(){
    // configure instanced array
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);
    
    for (unsigned int i = 0; i < rock->meshes.size(); i++){
        //when we declare a datatype as a vertex attribute that is greater than a vec4 things work a bit differently. The maximum amount of data allowed for a vertex attribute is equal to a vec4. Because a mat4 is basically 4 vec4s, we have to reserve 4 vertex attributes for this specific matrix. Because we assigned it a location of 3, the columns of the matrix will have vertex attribute locations of 3, 4, 5, and 6.
        GLuint VAO = rock->meshes[i].VAO;
        glBindVertexArray(VAO);
        // set attribute pointers for matrix (4 times vec4)
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
        glEnableVertexAttribArray(5);
        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
        glEnableVertexAttribArray(6);
        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));
        
        
        //This is a function for instancing, which means to update these attributes only at next instance and not at each vertex
        glVertexAttribDivisor(3, 1);
        glVertexAttribDivisor(4, 1);
        glVertexAttribDivisor(5, 1);
        glVertexAttribDivisor(6, 1);
        
        glBindVertexArray(0);
    }
}

Asteroid::Asteroid(){
    rock = new Model("Models/rock/rock.obj");
    asteroidShader = new Shader("Shaders/asteroidv.glsl","Shaders/asteroidf.glsl");
    amount = 1000;
    modelMatrices = new glm::mat4[amount];
    srand(glfwGetTime()); // initialize random seed
    float radius = 20.0;
    float offset = 2.5f;
    for(unsigned int i = 0; i < amount; i++)
    {
        glm::mat4 model = glm::mat4(1.0f);
        float angle = (float)i / (float)amount * 360.0f;
        float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float x = sin(angle) * radius + displacement;
        displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float y = displacement * 0.4f;
        displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float z = cos(angle) * radius + displacement;
        model = glm::translate(model, glm::vec3(10*x, 15*y, 10*z));
        
        float scale = (rand() % 20) / 100.0f + 0.05;
        model = glm::scale(model, glm::vec3(scale*10.0f));
        
        float rotAngle = (rand() % 360);
        model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));
        
        modelMatrices[i] = model;
        
        InitBuffers();
    }
}

void Asteroid::render(glm::mat4 view, glm::mat4 projection){
    asteroidShader->use();
    asteroidShader->setUniformMat4("projection", projection);
    asteroidShader->setUniformMat4("view", view);
    //rock->Draw(*asteroidShader);
//    asteroidShader->use();
    asteroidShader->setUniformInt("texture_diffuse1", 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, rock->textures_loaded[0].id);
    for (unsigned int i = 0; i < rock->meshes.size(); i++){
        glBindVertexArray(rock->meshes[i].VAO);
        glDrawElementsInstanced(GL_TRIANGLES, rock->meshes[i].indices.size(), GL_UNSIGNED_INT, 0, amount);
        glBindVertexArray(0);
    }
}
