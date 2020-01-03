#include "renderer.h"

#include <iostream>

#include <GL/glew.h>

#include "shader_object_pool.h"

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

  SDL_GL_SetSwapInterval(1);

  return true;
}

void Renderer::Destroy() {
  SDL_GL_DeleteContext(gl_context_);
  gl_context_ = NULL;
}

void Renderer::Clear() const {
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::Render() {
  
}
