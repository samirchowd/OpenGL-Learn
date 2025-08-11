#include "ElementBuffer.h"

ElementBuffer::ElementBuffer(const unsigned int* indices, size_t count, GLenum usage) 
    : ID(0), count(count) {
    glGenBuffers(1, &ID);
    bind();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), indices, usage);
}

ElementBuffer::~ElementBuffer() {
    if (ID != 0) {
        glDeleteBuffers(1, &ID);
    }
}

ElementBuffer::ElementBuffer(ElementBuffer&& other) noexcept 
    : ID(other.ID), count(other.count) {
    other.ID = 0;
    other.count = 0;
}

ElementBuffer& ElementBuffer::operator=(ElementBuffer&& other) noexcept {
    if (this != &other) {
        if (ID != 0) {
            glDeleteBuffers(1, &ID);
        }
        ID = other.ID;
        count = other.count;
        other.ID = 0;
        other.count = 0;
    }
    return *this;
}

void ElementBuffer::bind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
}

void ElementBuffer::unbind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void ElementBuffer::setData(const unsigned int* indices, size_t count, GLenum usage) {
    bind();
    this->count = count;
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), indices, usage);
}