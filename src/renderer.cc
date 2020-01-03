#include "renderer.h"

#include <iostream>

#include <GL/glew.h>

static GLuint program_id;
static GLint vertex_pos_location = -1;
static GLuint vertex_buffer_object = 0;
static GLuint index_buffer_object = 0;

static GLuint loadShader(GLenum type, const GLchar *source) {
  GLuint shader = glCreateShader(type);
  if (shader == 0) {
    return 0;
  }

  glShaderSource(shader, 1, &source, NULL);
  glCompileShader(shader);

  GLint compile_status;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_status);
  if (compile_status == GL_TRUE) {
    return shader;
  } else {
    GLint size;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &size);

    GLchar *log = new GLchar[size];
    glGetShaderInfoLog(shader, size, NULL, log);
    std::cerr << "GL shader failed: " << log << std::endl;
    delete[] log;
    
    glDeleteShader(shader);
    return 0;
  }
}

Renderer::Renderer() {
  
}

Renderer::~Renderer() {
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

  const GLchar *vertex_shader_source = ""
      "attribute vec4 aVertexPosition;"
      "void main() {"
        "gl_Position = aVertexPosition;"
      "}";
  GLuint vertex_shader = loadShader(GL_VERTEX_SHADER, vertex_shader_source);

  const GLchar *fragment_shader_source = ""
      "void main() {"
        "gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);"
      "}";
  GLuint fragment_shader = loadShader(GL_FRAGMENT_SHADER, fragment_shader_source);

  program_id = glCreateProgram();
  glAttachShader(program_id, vertex_shader);
  glAttachShader(program_id, fragment_shader);
  
  glLinkProgram(program_id);

  GLint link_status;
  glGetProgramiv(program_id, GL_LINK_STATUS, &link_status);
  if (link_status == GL_TRUE) {
    vertex_pos_location = glGetAttribLocation(program_id, "aVertexPosition");

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
  } else {
    GLint size;
    glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &size);

    GLchar *log = new GLchar[size];
    glGetProgramInfoLog(program_id, size, NULL, log);
    std::cerr << "GL program failed: " << log << std::endl;
    delete[] log;

    glDeleteProgram(program_id);
    program_id = 0;

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
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
  glUseProgram(program_id);

  glEnableVertexAttribArray(vertex_pos_location);

  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object);
  glVertexAttribPointer(vertex_pos_location, 2, GL_FLOAT, GL_FALSE, 0, 0);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_object);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

  glDisableVertexAttribArray(vertex_pos_location);

  glUseProgram(0);
}
