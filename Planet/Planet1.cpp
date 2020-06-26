#include<iostream>
#include<cmath>
#include<string>

//GLEW
#define GLEW_STATIC
#include<GL/glew.h>

//GLFW
#include<GLFW/glfw3.h>

#include "Utility/Shader.hpp"
#include "Utility/Camera.hpp"
#include "Utility/SphereGeneration.hpp"
#include "Utility/Textures.hpp"
#include "Utility/SkyBox.hpp"
#include "Utility/Atmosphere.hpp"
#include "Utility/Asteroid.hpp"

void processInput(GLFWwindow* window);
void framebuffer_size_callback(GLFWwindow* window, int screenwidth, int screenheight);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

//Window dimensions
const GLint WIDTH = 800, HEIGHT = 800;

GLfloat deltaTime = 0.0f;    // Time between current frame and last frame
GLfloat lastFrame = 0.0f; // Time of last frame

GLfloat lastX,lastY;
bool firstMouse = true;
float fov = 50.0f;

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
    glfwSetScrollCallback(window, scroll_callback);
    
    Sphere sphere;
    Shader object("Shaders/vshad.glsl","Shaders/fshad.glsl");
    Shader others("Shaders/otherv.glsl","Shaders/otherf.glsl");
    Shader sun("Shaders/sunv.glsl","Shaders/sunf.glsl");
    Texture diffuseMap("Images/earth/diffuse.jpg");
    Texture lightsMap("Images/earth/lights.jpg");
    Texture specularMap("Images/earth/specular.jpg");
    Texture dispMap("Images/earth/displacement.jpg");
    Texture moonMap("Images/others/moon.jpg");
    Texture venusMap("Images/others/venus.jpg");
    Texture marsMap("Images/others/mars.jpg");
    Texture sunMap("Images/sun.jpg");
    
    //Set depth test to less-than
    glDepthFunc(GL_LESS);
    //Enable depth testing
    glEnable(GL_DEPTH_TEST);
    //blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA , GL_ONE_MINUS_SRC_ALPHA);
    
    SkyBox skybox;
    
    glm::vec3 lightDirection = glm::vec3(1.0,1.4,-2.3);
    
    object.use();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::scale(model, glm::vec3(10,10,10));
    object.setUniformMat4("model", model);
    object.setUniformInt("material.diffuseMap", 0);
    object.setUniformInt("material.lightsMap", 1);
    object.setUniformInt("material.specularMap", 2);
    object.setUniformInt("dispMap", 3);
    object.setUniformVec3("dirLight.direction", lightDirection);
    
    glm::vec3 otherPos[3] = {
        glm::vec3(12.0f,5.0,6.0),
        glm::vec3(1.6f, 1.8f, -2.4f) * 40.0f,
        glm::vec3(-1.5f, -1.3f, -1.7f) * 80.0f
    };
    
    float sizes[3] = {1.4f, 9.8f, 6.0f};
    
    others.use();
    others.setUniformVec3("dirLight.direction", lightDirection);
    
    Atmosphere atmosphere;
    Asteroid asteroid;
    
    //Now we draw in the game loop
    while(!glfwWindowShouldClose(window)){
        if(glm::length(camera.GetCamPos()) >16.0f)
            camera.speed = 10.0f;
        else if(glm::length(camera.GetCamPos()) >11.0f)
            camera.speed = 2.0f;
        else if(glm::length(camera.GetCamPos())>10.0f)
            camera.speed = 0.1f * pow(20.0, glm::length(camera.GetCamPos()) - 10.0f);
        else
            camera.speed = 0.1f;
        
        
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
        glm::mat4 projection = glm::perspective((float)(glm::radians(fov)), (float)WIDTH/(float)HEIGHT, 0.1f, 1000.0f);
        
        
        
        //object
        object.use();
        diffuseMap.use();
        lightsMap.use(1);
        specularMap.use(2);
        dispMap.use(3);
        object.setUniformMat4("view", view);
        object.setUniformMat4("projection", projection);
        object.setUniformVec3("cameraPos", camera.GetCamPos());

        glBindVertexArray(sphere.VAO);
        glDrawElements(GL_TRIANGLES, sphere.indicesSize , GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        //moon
        others.use();
        for(int i=0;i<3; i++){
            if(i==0)
                moonMap.use();
            else if(i==1)
                venusMap.use();
            else
                marsMap.use();

            model = glm::mat4(1.0f);
            model = glm::translate(model, otherPos[i]);
            model = glm::scale(model, glm::vec3(sizes[i]));
            others.setUniformMat4("model", model);
            others.setUniformMat4("view", view);
            others.setUniformMat4("projection", projection);
            others.setUniformVec3("cameraPos", camera.GetCamPos());

            glBindVertexArray(sphere.VAO);
            glDrawElements(GL_TRIANGLES, sphere.indicesSize , GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);
        }
        
        //rock.Draw(asteroidShader);
        asteroid.render(view, projection);
        atmosphere.renderAtm(view, projection, lightDirection, glm::vec3(0.0f), fov);
        skybox.render(view, projection);
        
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

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
    if (fov >= 1.0f && fov <= 89.0f)
        fov -= yoffset;
    if (fov <= 1.0f)
        fov = 1.0f;
    if (fov >= 89.0f)
        fov = 89.0f;
}
