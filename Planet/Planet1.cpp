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
float fov = 70.0f;

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
    Texture diffuseMap("Images/earth/diffuse.jpg");
    Texture lightsMap("Images/earth/lights.jpg");
    Texture specularMap("Images/earth/specular.jpg");
    
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    //normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    //texture attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glBindVertexArray(0);
    
    //Set depth test to less-than
    glDepthFunc(GL_LESS);
    //Enable depth testing
    glEnable(GL_DEPTH_TEST);
    //blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA , GL_ONE_MINUS_SRC_ALPHA);
    
    //Generate CubeMap
    Shader skyboxShader("Shaders/sky_vshad.glsl","Shaders/sky_fshad.glsl");
    std::vector<std::string> faces{
        "Images/space/right.jpg",
        "Images/space/left.jpg",
        "Images/space/top.jpg",
        "Images/space/bottom.jpg",
        "Images/space/front.jpg",
        "Images/space/back.jpg"
    };
    SkyBox skybox(faces);
    skyboxShader.use();
    skyboxShader.setUniformInt("skybox", 0);
    
    glm::vec3 lightDirection = glm::vec3(1.0,0.0,0.0);
    
    object.use();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::scale(model, glm::vec3(10,10,10));
    object.setUniformMat4("model", model);
    object.setUniformInt("material.diffuseMap", 0);
    object.setUniformInt("material.lightsMap", 1);
    object.setUniformInt("material.specularMap", 2);
    object.setUniformVec3("dirLight.direction", lightDirection);
    
    Shader scatter("Shaders/scatter_vert.glsl", "Shaders/scatter_frag.glsl");
    glm::vec3 scatter_quad[] = {
        glm::vec3(-1.0,-1.0,-1.0),
        glm::vec3(-1.0, 1.0,-1.0),
        glm::vec3( 1.0,-1.0,-1.0),
        glm::vec3( 1.0, 1.0,-1.0),
        glm::vec3(-1.0, 1.0,-1.0),
        glm::vec3( 1.0,-1.0,-1.0)
    };

    GLuint quadVBO,quadVAO;
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);

    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(scatter_quad), scatter_quad, GL_STATIC_DRAW);
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    scatter.use();
    scatter.setUniformFloat("earth_radius", 10.0f);
    scatter.setUniformFloat("atm_radius", 10.0 * 67.0f/64.0f);

    //Now we draw in the game loop
    while(!glfwWindowShouldClose(window)){
        if(glm::length(camera.GetCamPos()) <10.3)
            camera.speed = 0.8f;
        else
            camera.speed = 2.0f;
            
            
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
        glm::mat4 projection = glm::perspective((float)(glm::radians(fov)), (float)WIDTH/(float)HEIGHT, 0.1f, 100.0f);
        
        //object
        object.use();
        diffuseMap.use();
        lightsMap.use(1);
        specularMap.use(2);
        object.setUniformMat4("view", view);
        object.setUniformMat4("projection", projection);
        object.setUniformVec3("cameraPos", camera.GetCamPos());
        
        glBindVertexArray(sphereVAO);
        glDrawElements(GL_TRIANGLES, sphere.indicesSize , GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        
        glBlendFunc(GL_ONE, GL_SRC_ALPHA);
        scatter.use();
        view = camera.GetViewMatrix();
        glm::vec3 center = glm::vec3(view * glm::vec4(0,0,0,1.0));
        scatter.setUniformVec3("earth_center", center);
        view = camera.GetViewMatrix();
        glm::vec3 lightdir = glm::vec3(view * glm::vec4(lightDirection, 0.0));
        lightdir = glm::normalize(lightdir);
        scatter.setUniformVec3("light_direction", lightdir);
        scatter.setUniformFloat("fov", fov);

        glBindVertexArray(quadVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
        glBlendFunc(GL_ONE, GL_ZERO);
        
        //In the end we generate the skybox (actually should be rendered first but we optimize it by setting it at maximum depth, so we can discard fragments which dont pass depth test)
        glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
        skyboxShader.use();
        view = glm::mat4(glm::mat3(camera.GetViewMatrix())); // remove translation from the view matrix to give feeling of infiniteness to skybox
        skyboxShader.setUniformMat4("view", view);
        skyboxShader.setUniformMat4("projection", projection);
        // skybox cube
        glBindVertexArray(skybox.skyVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, skybox.cubemapTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glDepthFunc(GL_LESS); // set depth function back to default
        
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
