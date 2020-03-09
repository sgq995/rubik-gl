#ifndef MESH_H_
#define MESH_H_

#include <vector>

#include <glm/glm.hpp>

#include "buffer.h"
#include "buffer_layout.h"
#include "renderer.h"
#include "shader.h"

class Mesh {
public:
  Mesh();
  virtual ~Mesh();

  void Scale(const glm::vec3 &scaling);
  void Translate(const glm::vec3 &translation);
  void Rotate(const glm::f32 &angle, const glm::vec3 &rotation);

  virtual void Draw(const Renderer &renderer);
  virtual void Draw(const Renderer &renderer, GLint model_view_location);

  const Buffer& vertex_buffer() const;
  
  const Buffer& index_buffer() const;

  const BufferLayout& layout() const;

  const glm::mat4& model_view() const;
  void set_model_view(const glm::mat4 &model_view);
  const GLfloat *model_view_ptr() const;

protected:
  void CreateVertexBuffer(GLuint index, const std::vector<GLfloat> &verticies);
  void CreateIndexBuffer(const std::vector<GLuint> &indicies);

private:
  Buffer vertex_buffer_;
  Buffer index_buffer_;
  BufferLayout layout_;
  glm::mat4 model_view_;
};

#endif  // MESH_H_
