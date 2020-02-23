#include "app.h"

#include <iostream>

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <SDL.h>

#include "buffer.h"
#include "buffer_layout.h"
#include "shader.h"
#include "window.h"

App::App() {
  system_status_ = SDL_Init(SDL_INIT_VIDEO);
}

App::~App() {
  SDL_Quit();
}


const char *App::GetError() const {
  return SDL_GetError();
}


void App::Start() {
  bool window_status = window_.Init({
      .title = "RubikGL", 
      .x = SDL_WINDOWPOS_CENTERED,
      .y = SDL_WINDOWPOS_CENTERED,
      .width = 800,
      .height = 600,
  });
  if (!window_status) {
    system_status_ = -1;
    return;
  }
  
  running_ = true;
  Run();
}

void App::Run() {
  Shader shader;
  bool shader_status = InitShader(&shader);
  if (!shader_status) {
    system_status_ = -1;
    return;
  }

  glm::mat4 model_view = glm::mat4(1.0f);
  model_view = glm::rotate(model_view, glm::radians(45.0f), glm::vec3(0.0, 0.0, 1.0));

  GLfloat vertex_data[8] = {
    -0.5f, -0.5f,
    0.5f, -0.5f,
    0.5f, 0.5f,
    -0.5f, 0.5f
  };

  Buffer vertex_buffer({
      .target = GL_ARRAY_BUFFER,
      .type = GL_FLOAT,
      .size = 2,
  }, {
      .data = vertex_data, 
      .size = sizeof(vertex_data) 
  });

  BufferLayout vertex_buffer_layout;
  vertex_buffer_layout.Attrib(shader.AttribLocation("aVertexPosition"), vertex_buffer);
  
  GLuint index_data[6] = {
    0, 1, 2, 0, 2, 3
  };

  Buffer index_buffer({
      .target = GL_ELEMENT_ARRAY_BUFFER,
      .type = GL_UNSIGNED_INT,
      .count = 6,
  }, {
      .data = index_data,
      .size = sizeof(index_data),
  });

  const Renderer &renderer = window_.renderer();

#define ADJUST_DELTA(delta, val, max, min) \
    ((val) > (max)) ? (-(delta)) : \
    ((val) < (min)) ? (-(delta)) : \
    (delta)

#define OVERFLOW(val, max, min) \
    ((val) > (max)) ? ((val) - (max) + (min)) : \
    ((val) < (min)) ? ((max) - (val) + (min)) : \
    (val)

  float color_red = 0.5f;
  float color_green = 0.3f;
  float color_blue = 0.8f;

  float color_red_delta = 0.05f;
  float color_green_delta = 0.03f;
  float color_blue_delta = 0.08f;

  SDL_Event event;
  while (running_) {
    // PollEvents
    if (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        running_ = false;
      }
    }

    // Update
    color_red_delta = ADJUST_DELTA(color_red_delta, color_red, 1.0f, 0.0f);
    // color_green_delta = ADJUST_DELTA(color_green_delta, color_green, 1.0f, 0.0f);
    // color_blue_delta = ADJUST_DELTA(color_blue_delta, color_blue, 1.0f, 0.0f);

    color_red = OVERFLOW(color_red + color_red_delta, 1.0f, 0.0f);
    // color_green = OVERFLOW(color_green + color_green_delta, 1.0f, 0.0f);
    // color_blue = OVERFLOW(color_blue + color_blue_delta, 1.0f, 0.0f);

    // Render
    window_.Clear();
    shader.Use();

    glUniformMatrix4fv(shader.UniformLocation("uModelView"), 1, GL_FALSE, glm::value_ptr(model_view));
    glUniform4f(shader.UniformLocation("uColor"), color_red, color_green, color_blue, 1.0f);

    vertex_buffer_layout.Bind();

    renderer.DrawElements(GL_TRIANGLES, index_buffer);
    GLenum error = glGetError();
    while (error != GL_NO_ERROR) {
      std::cerr << "[OpenGL] Error code (" << error << ')' << std::endl;
      error = glGetError();
    }

    window_.Swap();
  }
}


int App::system_status() const {
  return system_status_;
}


bool App::InitShader(Shader *shader) {
  ShaderObjectPool object_pool;
  ShaderSource source;

  const GLchar *vertex_shader_source = ""
      "uniform mat4 uModelView;"
      "attribute vec3 aVertexPosition;"
      "void main() {"
        "gl_Position = uModelView * vec4(aVertexPosition, 1.0f);"
      "}";
  source.type = GL_VERTEX_SHADER;
  source.code = vertex_shader_source;
  object_pool.Compile("vertex", source);

  const GLchar *fragment_shader_source = ""
      "uniform vec4 uColor;"
      "void main() {"
        "gl_FragColor = uColor;"
      "}";
  source.type = GL_FRAGMENT_SHADER;
  source.code = fragment_shader_source;
  object_pool.Compile(source);

  const ShaderObject vertex_object = object_pool.Get("vertex");
  shader->Attach(vertex_object);

  const ShaderObject fragment_object = object_pool.Get(fragment_shader_source);
  shader->Attach(fragment_object);
  
  return shader->Link();
}
