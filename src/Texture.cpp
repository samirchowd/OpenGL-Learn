#include "Texture.h"
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Texture::Texture(const std::string& imagePath, GLenum format) 
    : ID(0), width(0), height(0), channels(0) {
    glGenTextures(1, &ID);
    bind();
    
    // Set default parameters
    setParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
    setParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);
    setParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    setParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    // Load image using stb_image
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(imagePath.c_str(), &width, &height, &channels, 0);
    
    if (data) {
        GLenum internalFormat = format;
        GLenum dataFormat = format;
        
        // Auto-detect format based on channels
        if (channels == 1) {
            internalFormat = dataFormat = GL_RED;
        } else if (channels == 3) {
            internalFormat = dataFormat = GL_RGB;
        } else if (channels == 4) {
            internalFormat = dataFormat = GL_RGBA;
        }
        
        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        
        stbi_image_free(data);
    } else {
        std::cerr << "ERROR::TEXTURE::FAILED_TO_LOAD: " << imagePath << std::endl;
    }
}

Texture::~Texture() {
    if (ID != 0) {
        glDeleteTextures(1, &ID);
    }
}

Texture::Texture(Texture&& other) noexcept 
    : ID(other.ID), width(other.width), height(other.height), channels(other.channels) {
    other.ID = 0;
    other.width = 0;
    other.height = 0;
    other.channels = 0;
}

Texture& Texture::operator=(Texture&& other) noexcept {
    if (this != &other) {
        if (ID != 0) {
            glDeleteTextures(1, &ID);
        }
        ID = other.ID;
        width = other.width;
        height = other.height;
        channels = other.channels;
        
        other.ID = 0;
        other.width = 0;
        other.height = 0;
        other.channels = 0;
    }
    return *this;
}

void Texture::bind(unsigned int slot) const {
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, ID);
}

void Texture::unbind() const {
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::setParameter(GLenum pname, GLint param) {
    bind();
    glTexParameteri(GL_TEXTURE_2D, pname, param);
}