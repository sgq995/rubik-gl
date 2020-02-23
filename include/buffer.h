#ifndef BUFFER_H_
#define BUFFER_H_

#include <GL/glew.h>

class Buffer {
public:
  struct Properties {
    GLenum target;
    GLenum type;

    union {
      GLsizei count;
      GLint size;
    };
  };

  struct Pointer {
    const GLvoid *data;
    GLsizeiptr size;
  };

  Buffer();
  Buffer(const Buffer::Properties &props, const Buffer::Pointer &pointer);
  Buffer(const Buffer::Properties &props, const Buffer::Pointer &pointer, GLenum usage);
  ~Buffer();

  void Create(const Buffer::Properties &props, const Buffer::Pointer &pointer);
  void Create(const Buffer::Properties &props, const Buffer::Pointer &pointer, GLenum usage);

  void Bind() const;

  const GLenum& target() const;

  const GLenum& type() const;

  const GLsizei count() const;
  void set_count(const GLsizei &count);

  const GLint size() const;
  void set_size(const GLint &size);

  const GLuint& object() const;

private:
  GLenum target_;
  
  GLenum type_;
  union {
    GLsizei count_;
    GLint size_;
  };

  GLuint object_ = 0;
};

#endif  // BUFFER_H_
