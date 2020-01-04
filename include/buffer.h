#ifndef BUFFER_H_
#define BUFFER_H_

#include <GL/glew.h>

class Buffer {
public:
  Buffer(GLenum target, const GLvoid *data, GLsizeiptr size);
  Buffer(GLenum target, const GLvoid *data, GLsizeiptr size, GLenum usage);
  ~Buffer();

  void Bind();
  void DrawArrays(GLenum mode);
  void DrawElements(GLenum mode);

  const GLsizei& count() const;
  void set_count(const GLsizei &count);

  const GLenum& type() const;
  void set_type(const GLenum &type);

private:
  const GLenum kTarget_;
  GLuint object_ = 0;

  GLsizei count_ = 0;
  GLenum type_ = GL_NONE;
};

#endif  // BUFFER_H_
