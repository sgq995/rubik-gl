#include <iostream>

#include <GL/glew.h>

#include <SDL.h>

#include "config.h"

#include "buffer.h"
#include "renderer.h"
#include "shader.h"
#include "shader_object_pool.h"
#include "window.h"

#define PRINT_ERROR_MESSAGE(msg) \
    std::cerr << msg << " failed: " << SDL_GetError() << std::endl

enum return_code {
  OK,
  ERR_SDL_INIT,
  ERR_GLEW_INIT,
};

static GLint vertex_position_location = -1;
static GLint color_location = -1;
static GLuint vertex_array_object = 0;

#define ADJUST_DELTA(delta, val, max, min) \
    ((val) > (max)) ? (-(delta)) : \
    ((val) < (min)) ? (-(delta)) : \
    (delta)

#define OVERFLOW(val, max, min) \
    ((val) > (max)) ? ((val) - (max) + (min)) : \
    ((val) < (min)) ? ((max) - (val) + (min)) : \
    (val)

static float color_red = 0.5f;
static float color_green = 0.3f;
static float color_blue = 0.8f;

static float color_red_delta = 0.05f;
static float color_green_delta = 0.03f;
static float color_blue_delta = 0.08f;

int main(int argc, char **argv) {
  int status = SDL_Init(SDL_INIT_VIDEO);
  if (status < 0) {
    PRINT_ERROR_MESSAGE("SDL_Init");
    return ERR_SDL_INIT;
  }

  {
    Window::Properties properties = {
      .title = "RubikGL", 
      .x = SDL_WINDOWPOS_CENTERED,
      .y = SDL_WINDOWPOS_CENTERED,
      .width = 800,
      .height = 600,
    };
    Window window(properties);
    const Renderer &renderer = window.renderer();

    Shader shader;
    {
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
          "uniform vec4 uColor;"
          "void main() {"
            "gl_FragColor = uColor;"
          "}";
      source.type = GL_FRAGMENT_SHADER;
      source.code = fragment_shader_source;
      object_pool.Compile(source);

      const ShaderObject vertex_object = object_pool.Get("vertex");
      shader.Attach(vertex_object);

      const ShaderObject fragment_object = object_pool.Get(fragment_shader_source);
      shader.Attach(fragment_object);
      
      if (shader.Link()) {
        color_location = glGetUniformLocation(shader.program(), "uColor");

        vertex_position_location = glGetAttribLocation(shader.program(), "aVertexPosition");
      }
    }

    GLfloat vertex_data[8] = {
      -0.5f, -0.5f,
      0.5f, -0.5f,
      0.5f, 0.5f,
      -0.5f, 0.5f
    };

    if (GLEW_VERSION_3_0) {
      glGenVertexArrays(1, &vertex_array_object);
      glBindVertexArray(vertex_array_object);
    }

    Buffer vertex_buffer(GL_ARRAY_BUFFER, vertex_data, sizeof(vertex_data));

    if (GLEW_VERSION_3_0) {
      vertex_buffer.Bind();
      glEnableVertexAttribArray(vertex_position_location);
      glVertexAttribPointer(vertex_position_location, 2, GL_FLOAT, GL_FALSE, 0, 0);
    }
    
    GLuint index_data[6] = {
      0, 1, 2, 0, 2, 3
    };

    Buffer index_buffer(GL_ELEMENT_ARRAY_BUFFER, index_data, sizeof(index_data));
    index_buffer.set_count(6);
    index_buffer.set_type(GL_UNSIGNED_INT);

    SDL_Event event;

    while (true) {
      if (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
          break;
        }
      }

      renderer.Clear();

      shader.Bind();

      color_red_delta = ADJUST_DELTA(color_red_delta, color_red, 1.0f, 0.0f);
      // color_green_delta = ADJUST_DELTA(color_green_delta, color_green, 1.0f, 0.0f);
      // color_blue_delta = ADJUST_DELTA(color_blue_delta, color_blue, 1.0f, 0.0f);

      color_red = OVERFLOW(color_red + color_red_delta, 1.0f, 0.0f);
      // color_green = OVERFLOW(color_green + color_green_delta, 1.0f, 0.0f);
      // color_blue = OVERFLOW(color_blue + color_blue_delta, 1.0f, 0.0f);

      glUniform4f(color_location, color_red, color_green, color_blue, 1.0f);

      if (GLEW_VERSION_3_0) {
        glBindVertexArray(vertex_array_object);
      } else {
        vertex_buffer.Bind();
        glEnableVertexAttribArray(vertex_position_location);
        glVertexAttribPointer(vertex_position_location, 2, GL_FLOAT, GL_FALSE, 0, 0);
      }

      index_buffer.Bind();
      index_buffer.DrawElements(GL_TRIANGLES);

      if (GLEW_VERSION_3_0) {
        glBindVertexArray(0);
      } else {
        glDisableVertexAttribArray(vertex_position_location);
      }

      glUseProgram(0);

      GLenum error = glGetError();
      while (error != GL_NO_ERROR) {
        std::cerr << "[OpenGL] Error code (" << error << ')' << std::endl;
        error = glGetError();
      }

      window.Swap();
    }
  }

  SDL_Quit();

  return OK;
}
