#include "shader.h"

#include <iostream>

Shader::Shader() {
  program_ = glCreateProgram();
}

Shader::~Shader() {
  DetachShaders();
  glDeleteProgram(program_);
}


bool Shader::Attach(const ShaderObject &object) {
  glAttachShader(program_, object.id);

  return true;
}

bool Shader::Link() {
  if (linked_) {
    return true;
  }

  glLinkProgram(program_);

  GLint link_status;
  glGetProgramiv(program_, GL_LINK_STATUS, &link_status);
  if (link_status == GL_FALSE) {
    GLint size;
    glGetProgramiv(program_, GL_INFO_LOG_LENGTH, &size);

    GLchar *log = new GLchar[size];
    glGetProgramInfoLog(program_, size, NULL, log);
    std::cerr << "GL program failed: " << log << std::endl;
    delete[] log;

    DetachShaders();
    glDeleteProgram(program_);
    program_ = glCreateProgram();

    linked_ = false;
    return false;
  }

  linked_ = true;
  return true;
}


void Shader::Use() const {
  glUseProgram(program_);
}


GLint Shader::AttribLocation(const GLchar *name) {
  if (attribs_.find(name) == attribs_.end()) {
    attribs_[name] = glGetAttribLocation(program_, name);
  }

  return attribs_[name];
}

GLint Shader::UniformLocation(const GLchar *name) {
  if (uniforms_.find(name) == uniforms_.end()) {
    uniforms_[name] = glGetUniformLocation(program_, name);
  }

  return uniforms_[name];
}


const GLuint& Shader::program() const {
  return program_;
}


void Shader::DetachShaders() const {
  GLint attached_shaders = 0;
  glGetProgramiv(program_, GL_ATTACHED_SHADERS, &attached_shaders);

  GLuint *shaders = new GLuint[attached_shaders];
  glGetAttachedShaders(program_, attached_shaders, NULL, shaders);

  for (GLint i = 0; i < attached_shaders; ++i) {
    glDetachShader(program_, shaders[i]);
  }
}
