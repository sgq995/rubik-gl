#include <iostream>

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <SDL.h>

#include "config.h"

#include "buffer.h"
#include "buffer_layout.h"
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

static GLint model_view_location = -1;
static GLint vertex_position_location = -1;
static GLint color_location = -1;

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
    Window window({
        .title = "RubikGL", 
        .x = SDL_WINDOWPOS_CENTERED,
        .y = SDL_WINDOWPOS_CENTERED,
        .width = 800,
        .height = 600,
    });
    const Renderer &renderer = window.renderer();

    Shader shader;
    {
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
      shader.Attach(vertex_object);

      const ShaderObject fragment_object = object_pool.Get(fragment_shader_source);
      shader.Attach(fragment_object);
      
      if (shader.Link()) {
        model_view_location = glGetUniformLocation(shader.program(), "uModelView");
        color_location = glGetUniformLocation(shader.program(), "uColor");

        vertex_position_location = glGetAttribLocation(shader.program(), "aVertexPosition");
      }
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
    vertex_buffer_layout.Attrib(vertex_position_location, vertex_buffer);
    
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

    SDL_Event event;

    while (true) {
      if (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
          break;
        }
      }

      renderer.Clear();

      shader.Use();

      color_red_delta = ADJUST_DELTA(color_red_delta, color_red, 1.0f, 0.0f);
      // color_green_delta = ADJUST_DELTA(color_green_delta, color_green, 1.0f, 0.0f);
      // color_blue_delta = ADJUST_DELTA(color_blue_delta, color_blue, 1.0f, 0.0f);

      color_red = OVERFLOW(color_red + color_red_delta, 1.0f, 0.0f);
      // color_green = OVERFLOW(color_green + color_green_delta, 1.0f, 0.0f);
      // color_blue = OVERFLOW(color_blue + color_blue_delta, 1.0f, 0.0f);

      glUniformMatrix4fv(model_view_location, 1, GL_FALSE, glm::value_ptr(model_view));
      glUniform4f(color_location, color_red, color_green, color_blue, 1.0f);

      vertex_buffer_layout.Bind();

      renderer.DrawElements(GL_TRIANGLES, index_buffer);

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
