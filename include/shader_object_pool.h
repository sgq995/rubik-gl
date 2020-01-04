#ifndef SHADER_OBJECT_POOL_H_
#define SHADER_OBJECT_POOL_H_

#include <string>
#include <unordered_map>

#include <GL/glew.h>

struct ShaderSource {
  GLenum        type;
  const GLchar *code;
};

struct ShaderObject {
  GLuint id;
};

class ShaderObjectPool {
public:
  ShaderObjectPool();
  ~ShaderObjectPool();

  bool Compile(const ShaderSource &source);
  bool Compile(const std::string &file_path);
  bool Compile(const std::string &key, const ShaderSource &source);

  const ShaderObject& Get(const ShaderSource &source) const;
  const ShaderObject& Get(const GLchar *code) const;
  const ShaderObject& Get(const std::string &key) const;

  void Clear();

private:
  ShaderObject invalid_object_;

  std::unordered_map<std::string, ShaderObject> cache_;
};

#endif  // SHADER_OBJECT_POOL_H_
