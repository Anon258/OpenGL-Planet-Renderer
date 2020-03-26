#include<iostream>
#include<cmath>

//GLEW
#define GLEW_STATIC
#include<GL/glew.h>

//GLFW
#include<GLFW/glfw3.h>

#include "Utility/Shader.hpp"
#include "Utility/Camera.hpp"

void processInput(GLFWwindow* window);
void framebuffer_size_callback(GLFWwindow* window, int screenwidth, int screenheight);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

//Window dimensions
const GLint WIDTH = 800, HEIGHT = 600;

Shader* shader;

int depth = 8;
GLuint VBO, VAO;

GLfloat deltaTime = 0.0f;    // Time between current frame and last frame
GLfloat lastFrame = 0.0f; // Time of last frame

GLfloat lastX,lastY;
bool firstMouse = true;
float fov = 45.0f;

//Now we will build and compile our shaders

Camera camera;

void Recursion(int d, glm::mat4 trans){
    if(d>0){
        glm::mat4 new_trans = trans; //view = glm::mat4(1.0f);
        new_trans = glm::translate(new_trans, glm::vec3(0.0,1.0f,0.0));
        new_trans = glm::scale(new_trans, glm::vec3(0.5,0.5,0.5));
        GLuint transformLoc = glGetUniformLocation(shader->program, "transform");
        //glm::mat4 projection = glm::ortho(-1.0,1.0,-0.8,1.4,-1.0,1.0);
        glm::mat4 projection = glm::perspective((float)(glm::radians(fov)), (float)WIDTH/(float)HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        //view  = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
        glm::mat4 transformation = projection * view * new_trans;
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transformation));
    
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);
        Recursion(d-1, new_trans);
    
        new_trans = trans;
        new_trans = glm::translate(new_trans, glm::vec3((float)sin(2*M_PI/3), (float)cos(2*M_PI/3), 0.0f));
        new_trans = glm::scale(new_trans, glm::vec3(0.5,0.5,0.5));
        transformation = projection * view * new_trans;
        transformLoc = glGetUniformLocation(shader->program, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transformation));
    
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);
        Recursion(d-1, new_trans);

        new_trans = trans;
        new_trans = glm::translate(new_trans, glm::vec3((float)sin(4*M_PI/3), (float)cos(4*M_PI/3), 0.0f));
        new_trans = glm::scale(new_trans, glm::vec3(0.5,0.5,0.5));
        transformation = projection * view * new_trans;
        transformLoc = glGetUniformLocation(shader->program, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transformation));
    
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);
        Recursion(d-1, new_trans);
    }
}

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
    
    //Next we create a vertex data to draw
    GLfloat tri1[] =
    {
        //position              //color
        -(float)sin(2*M_PI/3), -(float)cos(2*M_PI/3), 0.0f,     1.0f, 0.0f, 0.0f,//bottom left
        -(float)sin(4*M_PI/3), -(float)cos(4*M_PI/3), 0.0f,      0.0f, 1.0f, 0.0f,//bottom right
        0.0f, -1.0f, 0.0f,                       0.0f, 0.0f, 1.0f//top
    };
    
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    
    shader = new Shader("Shaders/cube_vert_shader.glsl", "Shaders/cube_frag_shader.glsl");
    
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(tri1), tri1, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0,3, GL_FLOAT, GL_FALSE, 6*sizeof(GLfloat), (GLvoid*) 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1,3, GL_FLOAT, GL_FALSE, 6*sizeof(GLfloat), (GLvoid*)(3*sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    
    glBindVertexArray(0);
    
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
        
        shader->use();
        
        glm::mat4 trans = glm::mat4(1.0f); //view = glm::mat4(1.0f);
        trans = glm::scale(trans, glm::vec3(0.5,0.5,0.5));
        glm::mat4 view = camera.GetViewMatrix();
        //glm::mat4 projection = glm::ortho(-1.0,1.0,-0.8,1.4,-1.0,1.0);
        glm::mat4 projection = glm::perspective((float)(glm::radians(fov)), (float)WIDTH/(float)HEIGHT, 0.1f, 100.0f);
        //view  = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
        glm::mat4 transformation = projection * view * trans;
        GLuint transformLoc = glGetUniformLocation(shader->program, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transformation));
        
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);
        
        Recursion(depth - 1, trans);
        //swap screen buffers
        glfwSwapBuffers(window);
    }
    
    glDeleteVertexArrays(1, &VAO);  //We put them to 1 on line 128
    glDeleteBuffers(1, &VBO);
    
    glfwTerminate();
    
    return EXIT_SUCCESS;
}
//void processInput(GLFWwindow *window)
//{
//    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
//        glfwSetWindowShouldClose(window, true);
//}

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
