#define STB_IMAGE_IMPLEMENTATION
#include "Textures.hpp"

Texture::Texture(const GLchar* name){
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
    
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    //load and create textures
    GLint width, height, nchannels; //nchannels is for the number of color channels
    //stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(name, &width, &height, &nchannels, 0);
    std::cout<<width<<" "<<height<<" "<<nchannels<<std::endl;
    GLenum format = GL_RGB;
    if(nchannels == 1)
        format = GL_RED;
    else if(nchannels == 3)
        format = GL_RGB;
    else if(nchannels ==4)
        format = GL_RGBA;
    
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    if (data){
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else{
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
}

void Texture::use(int i){
    glActiveTexture(GL_TEXTURE0 + i);
    glBindTexture(GL_TEXTURE_2D, texture);
}
