#pragma once

#include <glad/glad.h>
#include <cstddef>

class VertexBuffer {
private:
    unsigned int ID;

public:
    VertexBuffer(const void* data, size_t size, GLenum usage = GL_STATIC_DRAW);
    ~VertexBuffer();
    
    // Rule of 5 - prevent copying, allow moving
    VertexBuffer(const VertexBuffer&) = delete;
    VertexBuffer& operator=(const VertexBuffer&) = delete;
    VertexBuffer(VertexBuffer&& other) noexcept;
    VertexBuffer& operator=(VertexBuffer&& other) noexcept;

    void bind() const;
    void unbind() const;
    void setData(const void* data, size_t size, GLenum usage = GL_STATIC_DRAW);
    unsigned int getID() const { return ID; }
};