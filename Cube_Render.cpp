#include<iostream>
#include<cmath>

//GLEW
#define GLEW_STATIC
#include<GL/glew.h>

//GLFW
#include<GLFW/glfw3.h>

#include "Utility/Cube.hpp"


void processInput(GLFWwindow* window);
void framebuffer_size_callback(GLFWwindow* window, int screenwidth, int screenheight);

//Window dimensions
const GLint WIDTH = 800, HEIGHT = 800;
int main(){
    //Initialise glfw
    glfwInit();
    
    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 1 );
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
    glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
    glfwWindowHint( GLFW_RESIZABLE, GL_FALSE );
    
    GLFWwindow *window = glfwCreateWindow( WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr );
    
    if (nullptr == window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        
        return EXIT_FAILURE;
    }
    
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    glewExperimental = GL_TRUE;
    if(GLEW_OK != glewInit()){
        std::cout<<"Failed to initialise GLEW"<<std::endl;
        
        return EXIT_FAILURE;
    }
    
    Cube cube1("Shaders/cube_vert_shader.glsl", "Shaders/cube_frag_shader.glsl");
    
    glClearDepth(1.0);
    //Set depth test to less-than
    glDepthFunc(GL_LESS);
    //Enable depth testing
    glEnable(GL_DEPTH_TEST);
    
    //Now we draw in the game loop
    while(!glfwWindowShouldClose(window)){
        
        processInput(window);
        //Check if any events have been activated - key press or something
        glfwPollEvents();
        
        //Render and Clear the color buffer
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        //matrix transformations to rotate
        glm::mat4 trans = glm::mat4(1.0f);
        trans = glm::scale(trans, glm::vec3(0.5f,0.5,0.5f));
        trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0));
        trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
        trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(1.0f, 1.0f, 1.0));
        
        cube1.shader->use();
        GLuint transformLoc = glGetUniformLocation(cube1.shader->program, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
        
        glBindVertexArray(cube1.VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        
        //swap screen buffers
        glfwSwapBuffers(window);
    }
    
    glfwTerminate();
    
    return EXIT_SUCCESS;
}

//To exit
void processInput(GLFWwindow* window){
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int screenwidth, int screenheight){
    glViewport(0, 0, screenwidth, screenheight);
}

