#ifndef BUFFER_LAYOU_H_
#define BUFFER_LAYOU_H_

#include <GL/glew.h>

#include "buffer.h"

class BufferLayout {
public:
  struct Parameters {
    GLboolean normalized;
    GLint stride;
    GLsizei offset;
  };

  BufferLayout();
  ~BufferLayout();

  void Attrib(GLuint index, const Buffer& buffer);
  void Attrib(GLuint index, const Buffer& buffer, const BufferLayout::Parameters &params);
  
  void Bind() const;

private:
  const BufferLayout::Parameters kDefaultParams = {
    .normalized = GL_FALSE,
    .stride = 0,
    .offset = 0,
  };

  GLuint object_ = 0;
};

#endif  // BUFFER_LAYOUT_H_
