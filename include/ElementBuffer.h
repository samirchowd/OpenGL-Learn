#pragma once

#include <glad/glad.h>
#include <cstddef>

class ElementBuffer {
private:
    unsigned int ID;
    size_t count;

public:
    ElementBuffer(const unsigned int* indices, size_t count, GLenum usage = GL_STATIC_DRAW);
    ~ElementBuffer();
    
    // Rule of 5 - prevent copying, allow moving
    ElementBuffer(const ElementBuffer&) = delete;
    ElementBuffer& operator=(const ElementBuffer&) = delete;
    ElementBuffer(ElementBuffer&& other) noexcept;
    ElementBuffer& operator=(ElementBuffer&& other) noexcept;

    void bind() const;
    void unbind() const;
    void setData(const unsigned int* indices, size_t count, GLenum usage = GL_STATIC_DRAW);
    size_t getCount() const { return count; }
    unsigned int getID() const { return ID; }
};