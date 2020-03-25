#include<iostream>
#include<cmath>

//GLEW
#define GLEW_STATIC
#include<GL/glew.h>

//GLFW
#include<GLFW/glfw3.h>

#include "Utility/Cube.hpp"

void framebuffer_size_callback(GLFWwindow* window, int screenwidth, int screenheight);

void Key_Rotation(GLFWwindow* window, int key, int scancode, int action, int mods);

//Window dimensions
const GLint WIDTH = 800, HEIGHT = 800;
GLfloat xrot,yrot,zrot, xpos, ypos, zpos;
GLint scrwidth, scrheight;

int mode = 0; //0 = Translate

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
    
    glfwSetKeyCallback(window, Key_Rotation);
    
    Cube cube1("Shaders/cube_vert_shader.glsl", "Shaders/cube_frag_shader.glsl");
    
    //Set depth test to less-than
    glDepthFunc(GL_LESS);
    //Enable depth testing
    glEnable(GL_DEPTH_TEST);
    
    //Now we draw in the game loop
    while(!glfwWindowShouldClose(window)){
        
        //Check if any events have been activated - key press or something
        glfwPollEvents();
        
        //Render and Clear the color buffer
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        //matrix transformations to rotate
        glm::mat4 trans = glm::mat4(1.0f), view = glm::mat4(1.0f);
        trans = glm::scale(trans, glm::vec3(0.5,0.5,0.5));
        trans = glm::translate(trans, glm::vec3(xpos, ypos, zpos));
        trans = glm::rotate(trans, xrot, glm::vec3(1.0f, 0.0f, 0.0f));
        trans = glm::rotate(trans, yrot, glm::vec3(0.0f, 1.0f, 0.0));
        trans = glm::rotate(trans, zrot, glm::vec3(0.0f, 0.0f, 1.0));
        //glm::mat4 projection = glm::ortho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
        glm::mat4 projection = glm::perspective((float)M_PI/4.0f, (float)WIDTH/(float)HEIGHT, 0.1f, 10.0f);
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
        trans = projection * view * trans;
        
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

void framebuffer_size_callback(GLFWwindow* window, int screenwidth, int screenheight){
    scrwidth = screenwidth;
    scrheight = screenheight;
    glViewport(0, 0, screenwidth, screenheight);
}

void Key_Rotation(GLFWwindow* window, int key, int scancode, int action, int mods){
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    else if (key == GLFW_KEY_R && action == GLFW_PRESS)
        mode = 1; //rotate
    else if (key == GLFW_KEY_T && action == GLFW_PRESS)
        mode = 0;
    else if (key == GLFW_KEY_LEFT && action == GLFW_PRESS){
        if(mode)
            yrot -= 0.5;
        else
            xpos -= 0.5;
    }
    else if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS){
        if(mode)
            yrot += 0.5;
        else
            xpos += 0.5;
    }
    else if (key == GLFW_KEY_DOWN && action == GLFW_PRESS){
        if(mode)
            xrot += 0.5;
        else
            ypos -= 0.5;
    }
    else if (key == GLFW_KEY_UP && action == GLFW_PRESS){
        if(mode)
            xrot -= 0.5;
        else
            ypos += 0.5;
    }
    else if (key == GLFW_KEY_COMMA && action == GLFW_PRESS){
        if(mode)
            zrot += 0.5;
        else
            zpos -= 0.5;
    }
    else if (key == GLFW_KEY_SLASH && action == GLFW_PRESS){
        if(mode)
            zrot -= 0.5;
        else
            zpos += 0.5;
    }
}

