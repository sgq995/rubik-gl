#include "shader_object_pool.h"

#include <iostream>

ShaderObjectPool::ShaderObjectPool()
    : invalid_object_({ 0 }) {

}

ShaderObjectPool::~ShaderObjectPool() {
  Clear();
}


bool ShaderObjectPool::Compile(const ShaderSource& source) {
  std::string key(static_cast<const char *>(source.code));
  return Compile(key, source);
}

bool ShaderObjectPool::Compile(const std::string& file_path) {
  ShaderSource source;

  // TODO: Populate source based on file contents

  return Compile(file_path, source);
}

bool ShaderObjectPool::Compile(const std::string& key, const ShaderSource& source) {
  GLuint id = glCreateShader(source.type);
  if (id == 0) {
    return false;
  }

  glShaderSource(id, 1, &source.code, NULL);
  glCompileShader(id);

  GLint compile_status;
  glGetShaderiv(id, GL_COMPILE_STATUS, &compile_status);
  if (compile_status == GL_FALSE) {
    GLint size;
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &size);

    GLchar *log = new GLchar[size];
    glGetShaderInfoLog(id, size, NULL, log);
    std::cerr << "GL shader compiling failed: " << log << std::endl;
    delete[] log;
    
    glDeleteShader(id);

    return false;
  }

  ShaderObject object = { id };
  cache_.insert(std::make_pair(key, object));

  return true;
}


const ShaderObject& ShaderObjectPool::Get(const ShaderSource& source) const {
  return Get(source.code);
}

const ShaderObject& ShaderObjectPool::Get(const GLchar *code) const {
  std::string key(static_cast<const char *>(code));
  return Get(key);
}

const ShaderObject& ShaderObjectPool::Get(const std::string& key) const {
  if (cache_.find(key) != cache_.end()) {
    return cache_.at(key);
  } else {
    return invalid_object_;
  }
}


void ShaderObjectPool::Clear() {
  for (auto pair : cache_) {
    const ShaderObject& object = pair.second;
    glDeleteShader(object.id);
  }

  cache_.clear();
}
