#ifndef SHADER_H_
#define SHADER_H_

#include <unordered_map>

#include <GL/glew.h>

#include "shader_object_pool.h"

class Shader {
public:
  Shader();
  ~Shader();

  bool Attach(const ShaderObject &source);
  bool Link();

  void Use() const;

  GLint AttribLocation(const GLchar *name);
  GLint UniformLocation(const GLchar *name);

  const GLuint& program() const;

private:
  void DetachShaders() const;

  bool linked_ = false;

  GLuint program_ = 0;

  std::unordered_map<const GLchar *, GLint> attribs_;
  std::unordered_map<const GLchar *, GLint> uniforms_;
};

#endif  // SHADER_H_
