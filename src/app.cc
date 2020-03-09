#include "app.h"

#include <iostream>

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <SDL.h>

#include "buffer.h"
#include "buffer_layout.h"
#include "cube.h"
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

  const Window::Properties props = {
      .title = "This is not RubikGL", 
      .x = SDL_WINDOWPOS_CENTERED,
      .y = SDL_WINDOWPOS_CENTERED,
      .width = 800,
      .height = 600,
  };
  bool window_status = window_.Create(props);
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

  double coord_x = 0.25;
  double coord_y = 0.5;
  double coord_z = 1.0;

  double coord_x_delta = 0.025;
  double coord_y_delta = 0.025;
  double coord_z_delta = 0.025;

  Uint32 change_color_time = SDL_GetTicks();
  Uint32 change_rotation_vector_time = SDL_GetTicks();

  glm::mat4 model_view = glm::mat4(1.0f);

  Cube cube(shader.AttribLocation("aVertexPosition"));
  cube.Translate(glm::vec3(-0.5f, 0.0f, 0.0f));
  cube.Scale(glm::vec3(0.3f, 0.3f, 0.3f));

  Cube my_cube(shader.AttribLocation("aVertexPosition"));
  my_cube.Translate(glm::vec3(0.5f, 0.0f, 0.0f));
  my_cube.Scale(glm::vec3(0.3f, 0.3f, 0.3f));

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

    if ((SDL_GetTicks() - change_rotation_vector_time) >= 50) {
      change_rotation_vector_time = SDL_GetTicks();

      coord_x_delta = ADJUST_DELTA(coord_x_delta, coord_x, 1.0, -1.0);
      coord_y_delta = ADJUST_DELTA(coord_y_delta, coord_y, 1.0, -1.0);
      coord_z_delta = ADJUST_DELTA(coord_z_delta, coord_z, 1.0, -1.0);

      coord_x = CLAMP(coord_x + coord_x_delta, 1.0, -1.0);
      coord_y = CLAMP(coord_y + coord_y_delta, 1.0, -1.0);
      coord_z = CLAMP(coord_z + coord_z_delta, 1.0, -1.0);
    }

    cube.Rotate(glm::radians(1.0f), glm::vec3(coord_x, coord_y, coord_z));
    my_cube.Rotate(glm::radians(-1.0f), glm::vec3(coord_x, coord_y, coord_z));
    
    // Render
    window_.Clear();
    shader.Use();

    glUniform4f(shader.UniformLocation("uColor"), color_red, color_green, color_blue, 1.0f);

    cube.Draw(renderer, shader.UniformLocation("uModelView"));
    my_cube.Draw(renderer, shader.UniformLocation("uModelView"));

    GLenum error = glGetError();
    while (error != GL_NO_ERROR) {
      std::cerr << "[OpenGL] Error code (" << error << ")" << std::endl;
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
          
          varying vec3 vVertexPosition;

          void main() {
            vVertexPosition = aVertexPosition;

            gl_Position = uModelView * vec4(aVertexPosition, 1.0f);
          }
      )glsl",
  };
  object_pool.Compile("vertex", vertex_shader_source);

  const ShaderSource fragment_shader_source = {
      .type = GL_FRAGMENT_SHADER,
      .code = R"glsl(
          uniform vec4 uColor;

          varying vec3 vVertexPosition;

          void main() {
            // gl_FragColor = uColor;
            // gl_FragColor = vec4(1);
            vec4 color = vec4(vVertexPosition + vec3(0.65, 0.65, 0.65), 1.0);
            gl_FragColor = color;
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
