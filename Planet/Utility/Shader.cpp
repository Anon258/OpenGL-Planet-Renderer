#include "Shader.hpp"

    
std::string Shader::readFile(const GLchar* path){
    std::string fileContent;
    
    try{
        std::ifstream fileStr(path, std::ios::in);
        
        if(!fileStr.is_open()){
            std::cerr<<"Couldn't open file"<<path<<" File doesn't exist"<<std::endl;
            return "";
        }
        
        std::stringstream fileStream;
        fileStream<<fileStr.rdbuf();
        fileStream<<"\0";
        fileStr.close();

        fileContent = fileStream.str().c_str();
    }
    catch(std::ifstream::failure e){
        std::cout<<"ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ"<<std::endl;
    }
        
    return fileContent;
}
    
Shader::Shader(const GLchar* vertPath, const GLchar* fragPath){
    GLuint vertexShader = CreateShaderGL(GL_VERTEX_SHADER, vertPath);
    GLuint fragmentShader = CreateShaderGL(GL_FRAGMENT_SHADER, fragPath);
    std::vector<GLuint> shaders;
    shaders.push_back(vertexShader);
    shaders.push_back(fragmentShader);
    
    program = CreateProgramGL(shaders);
    
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

GLuint Shader::CreateShaderGL(GLenum eShaderType, const GLchar* shaderPath){
    GLuint shader = glCreateShader(eShaderType);
    std::string shaderCode = readFile(shaderPath);
    const GLchar* shaderSource = (const GLchar *) shaderCode.c_str();
    glShaderSource(shader, 1, &shaderSource, NULL);
    glCompileShader(shader);
    
    //Now we check for any compilation errors
    GLint success;
    GLchar *infoLog = new GLchar[512];
    
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        
        const char* shaderType = NULL;
        switch(eShaderType){
            case GL_VERTEX_SHADER: shaderType = "VERTEX"; break;
            case GL_FRAGMENT_SHADER: shaderType = "FRAGMENT"; break;
        }
        std::cout<<"ERROR::SHADER::"<<shaderType<<"::COMPILATION_FAILED\n"<<infoLog<<std::endl;
    }
    delete[] infoLog;
    return shader;
}

GLuint Shader::CreateProgramGL(const std::vector<GLuint> &shaders){
    GLuint program = glCreateProgram();
    
    for(size_t i=0;i<shaders.size(); i++)
        glAttachShader(program, shaders[i]);
    
    glLinkProgram(program);
    
    GLint success;
    GLchar *infoLog = new GLchar[512];
    
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if(!success){
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        std::cout<<"ERROR::SHADER::PROGRAM::LINKING_FAILED\n"<<infoLog<<std::endl;
    }
    
    delete[] infoLog;
    return program;
}

void Shader::use(){
    glUseProgram(program);
}

void Shader::setUniformFloat(const GLchar* name, float value){
    GLint vertexLocation = glGetUniformLocation(program, name);
    glUniform1f(vertexLocation, value);
}

void Shader::setUniformInt(const GLchar* name, int value){
    GLint vertexLocation = glGetUniformLocation(program, name);
    glUniform1i(vertexLocation, value);
}

void Shader::setUniformMat4(const GLchar* name, glm::mat4 matrix){
    GLuint modelLoc = glGetUniformLocation(program, name);
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &matrix[0][0]);
}

void Shader::setUniformVec3(const GLchar* name, glm::vec3 vector){
    GLuint modelLoc = glGetUniformLocation(program, name);
    glUniform3f(modelLoc, vector.x, vector.y, vector.z);
}
