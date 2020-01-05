#include "buffer_layout.h"

#include <iostream>

BufferLayout::BufferLayout() {
  if (GLEW_VERSION_3_0) {
    glGenVertexArrays(1, &object_);
  } else {
    std::cerr << "[OpenGL] BufferLayout not supported yet" << std::endl;
  }
}

BufferLayout::~BufferLayout() {
  glDeleteVertexArrays(1, &object_);
}


void BufferLayout::Attrib(GLuint index, const Buffer& buffer) {
  Attrib(index, buffer, kDefaultParams);
}

void BufferLayout::Attrib(GLuint index, const Buffer& buffer, const BufferLayout::Parameters &params) {
  if (GLEW_VERSION_3_0) {
    glBindVertexArray(object_);
    buffer.Bind();
    glEnableVertexAttribArray(index);
    glVertexAttribPointer(index, buffer.size(), buffer.type(), params.normalized, params.stride, reinterpret_cast<const GLvoid*>(params.offset));
  } else {
    // TODO: Cache index, buff props and parameters 
  }
}


void BufferLayout::Bind() const {
  if (GLEW_VERSION_3_0) {
    glBindVertexArray(object_);
  } else {
    // TODO: Enable and set per index the vertex pointers
  }
}
