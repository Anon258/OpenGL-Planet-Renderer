#ifndef Shader_hpp
#define Shader_hpp

#include<fstream>
#include<string>
#include<sstream>
#include<iostream>
#include<vector>

#include<GL/glew.h>

class Shader{
public:
    GLuint program;
    
    std::string readFile(const GLchar* path);
    Shader(const GLchar* vertPath, const GLchar* fragPath);
    GLuint CreateShaderGL(GLenum eShaderType, const GLchar* shaderPath);
    GLuint CreateProgramGL(const std::vector<GLuint> &shaders);
    
    void use();
    void setUniformFloat(const GLchar* name, float value);
    void setUniformInt(const GLchar* name, int value);
};

#endif /* Shader_hpp */
