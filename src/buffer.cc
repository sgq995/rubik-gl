#include "buffer.h"

#include <iostream>

Buffer::Buffer(const Buffer::Properties &props, const Buffer::Pointer &pointer)
    : Buffer(props, pointer, GL_STATIC_DRAW) {

}

Buffer::Buffer(const Buffer::Properties &props, const Buffer::Pointer &pointer, GLenum usage)
    : target_(props.target), type_(props.type), count_(props.count) {
  glGenBuffers(1, &object_);
  glBindBuffer(target_, object_);
  glBufferData(target_, pointer.size, pointer.data, usage);
}

Buffer::~Buffer() {
  glDeleteBuffers(1, &object_);
}


void Buffer::Bind() const {
  glBindBuffer(target_, object_);
}


const GLenum& Buffer::target() const {
  return target_;
}


const GLenum& Buffer::type() const {
  return type_;
}


const GLsizei Buffer::count() const {
  return count_;
}

void Buffer::set_count(const GLsizei &count) {
  count_ = count;
}


const GLint Buffer::size() const {
  return size_;
}

void Buffer::set_size(const GLint &size) {
  size_ = size;
}


const GLuint& Buffer::object() const {
  return object_;
}
