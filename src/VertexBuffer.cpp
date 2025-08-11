#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(const void* data, size_t size, GLenum usage) : ID(0) {
    glGenBuffers(1, &ID);
    bind();
    glBufferData(GL_ARRAY_BUFFER, size, data, usage);
}

VertexBuffer::~VertexBuffer() {
    if (ID != 0) {
        glDeleteBuffers(1, &ID);
    }
}

VertexBuffer::VertexBuffer(VertexBuffer&& other) noexcept : ID(other.ID) {
    other.ID = 0;
}

VertexBuffer& VertexBuffer::operator=(VertexBuffer&& other) noexcept {
    if (this != &other) {
        if (ID != 0) {
            glDeleteBuffers(1, &ID);
        }
        ID = other.ID;
        other.ID = 0;
    }
    return *this;
}

void VertexBuffer::bind() const {
    glBindBuffer(GL_ARRAY_BUFFER, ID);
}

void VertexBuffer::unbind() const {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBuffer::setData(const void* data, size_t size, GLenum usage) {
    bind();
    glBufferData(GL_ARRAY_BUFFER, size, data, usage);
}