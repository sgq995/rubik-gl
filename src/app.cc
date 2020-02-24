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
  if (system_status_ < 0) {
    return;
  }

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
  shader.Use();

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
    ((val) >= (max)) ? (-(delta)) : \
    ((val) <= (min)) ? (-(delta)) : \
    (delta)

#define OVERFLOW(val, max, min) \
    ((val) > (max)) ? ((val) - (max) + (min)) : \
    ((val) < (min)) ? ((max) - (val) + (min)) : \
    (val)

#define CLAMP(val, max, min) \
    ((val) > (max)) ? (max) : \
    ((val) < (min)) ? (min) : \
    (val)

  float color_red = 0.5f;
  float color_green = 0.3f;
  float color_blue = 0.8f;

  float color_red_delta = 0.05f;
  float color_green_delta = 0.03f;
  float color_blue_delta = 0.08f;

  Uint32 change_color_time = SDL_GetTicks();

  SDL_Event event;
  while (running_) {
    // PollEvents
    if (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        running_ = false;
      }
    }

    // Update
    if ((SDL_GetTicks() - change_color_time) >= 100) {
      change_color_time = SDL_GetTicks();

      color_red_delta = ADJUST_DELTA(color_red_delta, color_red, 1.0f, 0.0f);
      color_green_delta = ADJUST_DELTA(color_green_delta, color_green, 1.0f, 0.0f);
      color_blue_delta = ADJUST_DELTA(color_blue_delta, color_blue, 1.0f, 0.0f);

      color_red = CLAMP(color_red + color_red_delta, 1.0f, 0.0f);
      color_green = CLAMP(color_green + color_green_delta, 1.0f, 0.0f);
      color_blue = CLAMP(color_blue + color_blue_delta, 1.0f, 0.0f);
    }

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
  
  const ShaderSource vertex_shader_source = {
      .type = GL_VERTEX_SHADER,
      .code = R"glsl(
          uniform mat4 uModelView;
          attribute vec3 aVertexPosition;
          
          void main() {
            gl_Position = uModelView * vec4(aVertexPosition, 1.0f);
          }
      )glsl",
  };
  object_pool.Compile("vertex", vertex_shader_source);

  const ShaderSource fragment_shader_source = {
      .type = GL_FRAGMENT_SHADER,
      .code = R"glsl(
          uniform vec4 uColor;

          void main() {
            gl_FragColor = uColor;
          }
      )glsl",
  };
  object_pool.Compile(fragment_shader_source);

  const ShaderObject vertex_object = object_pool.Get("vertex");
  shader->Attach(vertex_object);

  const ShaderObject fragment_object = object_pool.Get(fragment_shader_source.code);
  shader->Attach(fragment_object);
  
  return shader->Link();
}
