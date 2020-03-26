#include<iostream>
#include<cmath>

//GLEW
#define GLEW_STATIC
#include<GL/glew.h>

//GLFW
#include<GLFW/glfw3.h>

#include "Utility/Cube.hpp"
#include "Utility/Camera.hpp"

void processInput(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow* window, int screenwidth, int screenheight);
void Key_Rotation(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

//Window dimensions
const GLint WIDTH = 800, HEIGHT = 800;
GLfloat xrot,yrot,zrot, xpos, ypos, zpos, fov=45.0f;
GLint scrwidth, scrheight;

GLfloat deltaTime = 0.0f;    // Time between current frame and last frame
GLfloat lastFrame = 0.0f; // Time of last frame

int mode = 0; //0 = Translate

bool firstMouse = true;
float lastX =  WIDTH / 2.0;
float lastY =  HEIGHT / 2.0;

Camera camera;

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
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    
    Cube cube1("Shaders/cube_vert_shader.glsl", "Shaders/cube_frag_shader.glsl");
    
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
        
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        
        //matrix transformations to rotate
        glm::mat4 trans = glm::mat4(1.0f), view = glm::mat4(1.0f);
        trans = glm::scale(trans, glm::vec3(0.5,0.5,0.5));
        trans = glm::translate(trans, glm::vec3(xpos, ypos, zpos));
        trans = glm::rotate(trans, xrot, glm::vec3(1.0f, 0.0f, 0.0f));
        trans = glm::rotate(trans, yrot, glm::vec3(0.0f, 1.0f, 0.0));
        trans = glm::rotate(trans, zrot, glm::vec3(0.0f, 0.0f, 1.0));
        //glm::mat4 projection = glm::ortho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
        glm::mat4 projection = glm::perspective((float)(glm::radians(fov)), (float)WIDTH/(float)HEIGHT, 0.1f, 100.0f);
        
        view = camera.GetViewMatrix();
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
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS){
        if(mode)
            yrot -= 2*deltaTime;
        else
            xpos -= 2*deltaTime;
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS){
        if(mode)
            yrot += 2*deltaTime;
        else
            xpos += 2*deltaTime;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS){
        if(mode)
            xrot += 2*deltaTime;
        else
            ypos -= 2*deltaTime;
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS){
        if(mode)
            xrot -= 2*deltaTime;
        else
            ypos += 2*deltaTime;
    }
    if (glfwGetKey(window, GLFW_KEY_COMMA) == GLFW_PRESS){
        if(mode)
            zrot += 2*deltaTime;
        else
            zpos -= 2*deltaTime;
    }
    if (glfwGetKey(window, GLFW_KEY_SLASH) == GLFW_PRESS){
        if(mode)
            zrot -= 2*deltaTime;
        else
            zpos += 2*deltaTime;
    }
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS){
        fov -= 20.0f * deltaTime;
        if(fov<1.0f)
            fov = 1.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS){
        fov +=20.0f *deltaTime;
        if(fov>89.0f)
            fov=89.0f;
    }
    
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.KeyboardInput(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.KeyboardInput(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.KeyboardInput(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.KeyboardInput(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
        camera.KeyboardInput(DOWN, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
        camera.KeyboardInput(UP, deltaTime);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if(firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;
    
    camera.MouseMovement(xoffset, yoffset);
    
}

