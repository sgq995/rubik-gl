#include "buffer.h"

#include <iostream>

Buffer::Buffer(GLenum target, const GLvoid *data, GLsizeiptr size)
    : Buffer(target, data, size, GL_STATIC_DRAW) {

}

Buffer::Buffer(GLenum target, const GLvoid *data, GLsizeiptr size, GLenum usage)
    : kTarget_(target) {
  glGenBuffers(1, &object_);
  glBindBuffer(kTarget_, object_);
  glBufferData(kTarget_, size, data, usage);
}

Buffer::~Buffer() {
  glDeleteBuffers(1, &object_);
}


void Buffer::Bind() {
  glBindBuffer(kTarget_, object_);
}

void Buffer::DrawArrays(GLenum mode) {
  glDrawArrays(mode, 0, count_);
}

void Buffer::DrawElements(GLenum mode) {
  glDrawElements(mode, count_, type_, NULL);
}


const GLsizei& Buffer::count() const {
  return count_;
}

void Buffer::set_count(const GLsizei &count) {
  count_ = count;
}


const GLenum& Buffer::type() const {
  return type_;
}

void Buffer::set_type(const GLenum &type) {
  type_ = type;
}
