#include "renderer.h"

#include <iostream>

#include <GL/glew.h>

#include "shader_object_pool.h"

static GLint vertex_pos_location = -1;
static GLuint vertex_buffer_object = 0;
static GLuint index_buffer_object = 0;

Renderer::Renderer() {

}

Renderer::~Renderer() {
  delete shader_;
  Destroy();
}


bool Renderer::Create(SDL_Window *window) {
  gl_context_ = SDL_GL_CreateContext(window);
  if (gl_context_ == NULL) {
    return false;
  }

  GLenum glew_status = glewInit();
  if (glew_status != GLEW_OK) {
    std::cerr << "glewInit failed: " << glewGetErrorString(glew_status) << std::endl;
    return false;
  }

  shader_ = new Shader();

  ShaderObjectPool object_pool;
  ShaderSource source;

  const GLchar *vertex_shader_source = ""
      "attribute vec4 aVertexPosition;"
      "void main() {"
        "gl_Position = aVertexPosition;"
      "}";
  source.type = GL_VERTEX_SHADER;
  source.code = vertex_shader_source;
  object_pool.Compile("vertex", source);

  const GLchar *fragment_shader_source = ""
      "void main() {"
        "gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);"
      "}";
  source.type = GL_FRAGMENT_SHADER;
  source.code = fragment_shader_source;
  object_pool.Compile(source);

  const ShaderObject vertex_object = object_pool.Get("vertex");
  shader_->Attach(vertex_object);

  std::string fragment_key(static_cast<const char *>(fragment_shader_source));
  const ShaderObject fragment_object = object_pool.Get(fragment_key);
  shader_->Attach(fragment_object);
  
  if (shader_->Link()) {
    vertex_pos_location = glGetAttribLocation(shader_->program(), "aVertexPosition");

    GLfloat vertex_data[8] = {
      -0.5f, -0.5f,
      0.5f, -0.5f,
      0.5f, 0.5f,
      -0.5f, 0.5f
    };

    glGenBuffers(1, &vertex_buffer_object);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, GL_STATIC_DRAW);

    GLuint index_data[6] = {
      0, 1, 2, 0, 2, 3
    };

    glGenBuffers(1, &index_buffer_object);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_object);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index_data), index_data, GL_STATIC_DRAW);
  }

  return true;
}

void Renderer::Destroy() {
  SDL_GL_DeleteContext(gl_context_);
  gl_context_ = NULL;
}

void Renderer::Clear() {
  glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::Render() {
  shader_->Bind();

  glEnableVertexAttribArray(vertex_pos_location);

  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object);
  glVertexAttribPointer(vertex_pos_location, 2, GL_FLOAT, GL_FALSE, 0, 0);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_object);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

  glDisableVertexAttribArray(vertex_pos_location);

  glUseProgram(0);
}
