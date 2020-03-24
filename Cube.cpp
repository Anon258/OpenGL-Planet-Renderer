#include "Utility/Cube.hpp"

void processInput(GLFWwindow* window);
void framebuffer_size_callback(GLFWwindow* window, int screenwidth, int screenheight);

//Window dimensions
const GLint WIDTH = 800, HEIGHT = 600;

glm::vec4 positions[8] = {
    glm::vec4(-0.5f, -0.5f, 0.5f, 1.0f), //front bottom left
    glm::vec4(0.5f, -0.5f, 0.5f, 1.0f), //front bottom right
    glm::vec4(0.5f, 0.5f, 0.5f, 1.0f), //front top right
    glm::vec4(-0.5f, 0.5f, 0.5f, 1.0f), //front top left
    glm::vec4(-0.5f, -0.5f, -0.5f, 1.0f), //back bottom left
    glm::vec4(0.5f, -0.5f, -0.5f, 1.0f), //back bottom right
    glm::vec4(0.5f, 0.5f, -0.5f, 1.0f), //back top right
    glm::vec4(-0.5f, 0.5f, -0.5f, 1.0f) //back top left
};

glm::vec4 colors[8] = {
    glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), //front bottom left
    glm::vec4(1.0f, 0.0f, 1.0f, 1.0f), //front bottom right
    glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), //front top right
    glm::vec4(0.0f, 1.0f, 1.0f, 1.0f), //front top left
    glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), //back bottom left
    glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), //back bottom right
    glm::vec4(1.0f, 1.0f, 0.0f, 1.0f), //back top right
    glm::vec4(0.0f, 1.0f, 0.0f, 1.0f) //back top left
    
};

glm::vec4 glPoints[36];
glm::vec4 glColors[36];

int pos_index = 0;

void face(int a, int b, int c, int d){
    glPoints[pos_index] = positions[a]; glColors[pos_index] = colors[a]; pos_index++;
    glPoints[pos_index] = positions[b]; glColors[pos_index] = colors[b]; pos_index++;
    glPoints[pos_index] = positions[c]; glColors[pos_index] = colors[c]; pos_index++;
    glPoints[pos_index] = positions[a]; glColors[pos_index] = colors[a]; pos_index++;
    glPoints[pos_index] = positions[c]; glColors[pos_index] = colors[c]; pos_index++;
    glPoints[pos_index] = positions[d]; glColors[pos_index] = colors[d]; pos_index++;
}

void colorcube(){
    face(0,1,2,3); //front face
    face(0,1,5,4); //bottom face
    face(4,5,6,7); //back face
    face(3,2,6,7); //top face
    face(0,3,7,4); //left face
    face(1,2,6,5); //right face
};

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
    
    //Now we will build and compile our shaders
    Shader shader("Shaders/vertex_shader.glsl", "Shaders/fragment_shader.glsl");
    
    //Next we create a vertex data to draw
    
    colorcube();
    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glPoints) + sizeof(glColors), NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glPoints), glPoints);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(glPoints), sizeof(glColors), glColors);
    
    glVertexAttribPointer(0,4, GL_FLOAT, GL_FALSE, 4*sizeof(GLfloat), (GLvoid*) 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1,4, GL_FLOAT, GL_FALSE, 4*sizeof(GLfloat), (GLvoid*)(sizeof(glPoints)));
    glEnableVertexAttribArray(1);
    
    glBindVertexArray(0);
    
    //Now we draw in the game loop
    while(!glfwWindowShouldClose(window)){
        
        processInput(window);
        //Check if any events have been activated - key press or something
        glfwPollEvents();
        
        //Render and Clear the color buffer
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        shader.use();
        
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        
        //swap screen buffers
        glfwSwapBuffers(window);
    }
    
    glDeleteVertexArrays(1, &VAO);  //We put them to 1 on line 128
    glDeleteBuffers(1, &VBO);
    
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
