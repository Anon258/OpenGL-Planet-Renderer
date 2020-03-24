//CHECK LECTURE_00 FOR MOST OF THE EXPLANATIONS - SOME ARE PROVIDED BELOW (which were not part of Lecture_00 code


#include <iostream>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;

int main()
{
    // Init GLFW
    glfwInit();
    
    // Set all the required options for GLFW
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
    glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
    
    glfwWindowHint( GLFW_RESIZABLE, GL_FALSE );
    
    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow *window = glfwCreateWindow( WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr );
    
    int screenWidth, screenHeight;
    glfwGetFramebufferSize( window, &screenWidth, &screenHeight );
    
    if (nullptr == window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        
        return -1;
    }
    
    glfwMakeContextCurrent(window);
    
    glewExperimental = GL_TRUE; //It is related to some settings of glew. Some features which core opengl profile uses, are only available in experimental mode
    
    if(GLEW_OK != glewInit()){
        std::cout<<"Failed to initialise GLEW"<<std::endl;
        
        return -1;
    }
    
    glViewport( 0, 0, screenWidth, screenHeight );
    
    while(!glfwWindowShouldClose(window)){   //while window open, repeat
        
        glfwPollEvents();  //This checks for events like resize, mouse click or key press and
                           // calls required functions based on what is done
        
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);  //RGB and alpha
        glClear(GL_COLOR_BUFFER_BIT); //The glClear(GL_COLOR_BUFFER_BIT) just tells the system that the color buffer (which is the buffer which stores color at each pixel) is to be rewritten over
        
        //The color for this rewriting is specified in glClearColor()
        
        glfwSwapBuffers(window); //Check below for why we swap buffers (screens)
        
        //Rendering Mechanism: opengl (and typically any good rendering system) does rendering is that there are 2 screens (buffers). One screen is set to be displayed, the other you can draw on. In each game loop you draw your things on the not displayed screen. Then you swap the settings to display the non displayed screen and drawing on the displayed screen.
    }
    glfwTerminate();
    return 0;
}
