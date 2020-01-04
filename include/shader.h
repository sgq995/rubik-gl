#ifndef SHADER_H_
#define SHADER_H_

#include <GL/glew.h>

#include "shader_object_pool.h"

class Shader {
public:
  Shader();
  ~Shader();

  bool Attach(const ShaderObject &source);
  bool Link();

  void Use();

  const GLuint& program() const;

private:
  GLuint program_ = 0;
};

#endif  // SHADER_H_
