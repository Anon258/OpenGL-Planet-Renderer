#ifndef Textures_hpp
#define Textures_hpp

#include<string>
#include<iostream>
#include "stb_image.h"
#include<GL/glew.h>

class Texture{
    GLuint texture;
public:
    Texture(const GLchar* name);
    
    void use(int i=0);
};

#endif /* Textures_hpp */
