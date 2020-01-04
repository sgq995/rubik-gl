#include "shader.h"

#include <iostream>

Shader::Shader() {
  program_ = glCreateProgram();
}

Shader::~Shader() {
  glDeleteProgram(program_);
}


bool Shader::Attach(const ShaderObject &object) {
  glAttachShader(program_, object.id);

  return true;
}

bool Shader::Link() {
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

    glDeleteProgram(program_);
    program_ = glCreateProgram();

    return false;
  }

  return true;
}


void Shader::Use() {
  glUseProgram(program_);
}


const GLuint& Shader::program() const {
  return program_;
}
