#include "mesh.h"

#include <iostream>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Mesh::Mesh() 
    : model_view_(1.0f) {

}

Mesh::~Mesh() {

}


void Mesh::Scale(const glm::vec3 &scaling) {
  model_view_ = glm::scale(model_view_, scaling);
}

void Mesh::Translate(const glm::vec3 &translation) {
  model_view_ = glm::translate(model_view_, translation);
}

void Mesh::Rotate(const glm::f32 &angle, const glm::vec3 &rotation) {
  model_view_ = glm::rotate(model_view_, angle, rotation);
}


void Mesh::Draw(const Renderer &renderer) {
  layout_.Bind();
  renderer.DrawElements(GL_TRIANGLES, index_buffer_);
}

void Mesh::Draw(const Renderer &renderer, GLint model_view_location)
{
  ::glUniformMatrix4fv(model_view_location, 1, GL_FALSE, model_view_ptr());
  Draw(renderer);
}


const Buffer& Mesh::vertex_buffer() const {
  return vertex_buffer_;
}
  

const Buffer& Mesh::index_buffer() const {
  return index_buffer_;
}


const BufferLayout& Mesh::layout() const {
  return layout_;
}


const glm::mat4& Mesh::model_view() const {
  return model_view_;
}

void Mesh::set_model_view(const glm::mat4 &model_view) {
  model_view_ = model_view;
}

const GLfloat *Mesh::model_view_ptr() const {
  return glm::value_ptr(model_view_);
}


void Mesh::CreateVertexBuffer(GLuint index, const std::vector<GLfloat> &verticies) {
  Buffer::Properties props = {
    .target = GL_ARRAY_BUFFER,
    .type = GL_FLOAT,
    .size = 3,
  };

  Buffer::Pointer ptr = {
    .data = verticies.data(),
    .size = static_cast<GLsizeiptr>(verticies.size() * sizeof(GLfloat)),
  };
  
  vertex_buffer_.Create(props, ptr);
  layout_.Attrib(index, vertex_buffer_);
}

void Mesh::CreateIndexBuffer(const std::vector<GLuint> &indicies) {
  Buffer::Properties props = {
    .target = GL_ELEMENT_ARRAY_BUFFER,
    .type = GL_UNSIGNED_INT,
    .count = static_cast<GLsizei>(indicies.size()),
  };

  Buffer::Pointer ptr = {
    .data = indicies.data(),
    .size = static_cast<GLsizeiptr>(indicies.size() * sizeof(GLfloat)),
  };

  index_buffer_.Create(props, ptr);
}
