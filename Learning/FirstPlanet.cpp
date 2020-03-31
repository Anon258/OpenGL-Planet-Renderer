#include<iostream>
#include<cmath>

//GLEW
#define GLEW_STATIC
#include<GL/glew.h>

//GLFW
#include<GLFW/glfw3.h>

#include "Utility/Shader.hpp"
#include "Utility/Camera.hpp"
#include "Utility/SphereGeneration.hpp"

void processInput(GLFWwindow* window);
void framebuffer_size_callback(GLFWwindow* window, int screenwidth, int screenheight);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

//Window dimensions
const GLint WIDTH = 800, HEIGHT = 600;

GLfloat deltaTime = 0.0f;    // Time between current frame and last frame
GLfloat lastFrame = 0.0f; // Time of last frame

GLfloat lastX,lastY;
bool firstMouse = true;
float fov = 45.0f;

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
    
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    
    
    Sphere sphere;
    Shader object("Shaders/basiclighting_vert.glsl","Shaders/basiclighting_frag.glsl");
    Shader light("Shaders/light_vert.glsl","Shaders/light_frag.glsl");
    
    GLuint VBO, sphereVAO, EBO;
    glGenVertexArrays(1, &sphereVAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    
    glBindVertexArray(sphereVAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sphere.positionsSize*sizeof(float), &sphere.positions[0], GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sphere.indicesSize*sizeof(unsigned int), &sphere.indices[0], GL_STATIC_DRAW);
    
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    //normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);
    
    //Now for the light
    GLuint lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    //Set depth test to less-than
    glDepthFunc(GL_LESS);
    //Enable depth testing
    glEnable(GL_DEPTH_TEST);
    
    object.use();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::scale(model, glm::vec3(3.0f));
    object.setUniformMat4("model", model);
    object.setUniformVec3("objectColor", glm::vec3(1.0f,0.5f,0.31f));
    object.setUniformVec3("lightColor", glm::vec3(1.0f,1.0f,1.0f));
    
//    glm::vec3 lightPos(4.7f,3.6f, 4.0f);
//    object.setUniformVec3("lightPos", lightPos);
//
//    light.use();
//    model = glm::mat4(1.0f);
//    light.setUniformMat4("model", model);
    
    //Now we draw in the game loop
    while(!glfwWindowShouldClose(window)){
        
        processInput(window);
        //Check if any events have been activated - key press or something
        glfwPollEvents();
        
        //Render and Clear the color buffer
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective((float)(glm::radians(fov)), (float)WIDTH/(float)HEIGHT, 0.1f, 200.0f);
        
        //object
        object.use();
        object.setUniformMat4("view", view);
        object.setUniformMat4("projection", projection);
        object.setUniformVec3("viewPos", camera.GetCamPos());
        
        glm::vec3 lightPos(7.0f*(float)cos(currentFrame),3.7*(float)cos(currentFrame)*(float)sin(currentFrame)+ 8.3*(float)sin(currentFrame), 4.0f*(float)sin(currentFrame));
        object.setUniformVec3("lightPos", lightPos);
        
        glBindVertexArray(sphereVAO);
        glDrawElements(GL_TRIANGLES, sphere.indicesSize , GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        
        light.use();
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.1f));
        light.setUniformMat4("model", model);
        light.setUniformMat4("view", view);
        light.setUniformMat4("projection", projection);
        
        glBindVertexArray(lightVAO); 
        glDrawElements(GL_TRIANGLES, sphere.indicesSize , GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        
        //swap screen buffers
        glfwSwapBuffers(window);
    }
    
    glfwTerminate();
    
    return EXIT_SUCCESS;
}


void framebuffer_size_callback(GLFWwindow* window, int screenwidth, int screenheight){
    glViewport(0, 0, screenwidth, screenheight);
}

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
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
