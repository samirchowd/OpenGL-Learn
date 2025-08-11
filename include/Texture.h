#pragma once

#include <glad/glad.h>
#include <string>

class Texture {
private:
    unsigned int ID;
    int width, height, channels;

public:
    Texture(const std::string& imagePath, GLenum format = GL_RGB);
    ~Texture();
    
    // Rule of 5 - prevent copying, allow moving
    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;
    Texture(Texture&& other) noexcept;
    Texture& operator=(Texture&& other) noexcept;

    void bind(unsigned int slot = 0) const;
    void unbind() const;
    void setParameter(GLenum pname, GLint param);
    unsigned int getID() const { return ID; }
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    int getChannels() const { return channels; }
};