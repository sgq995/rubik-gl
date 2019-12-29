#include "renderer.h"

Renderer::Renderer() {

}

Renderer::~Renderer() {
  Destroy();
}


bool Renderer::Create(SDL_Window *window) {
  gl_context_ = SDL_GL_CreateContext(window);
  return (gl_context_ == NULL) ? false : true;
}

void Renderer::Destroy() {
  SDL_GL_DeleteContext(gl_context_);
  gl_context_ = NULL;
}

void Renderer::Clear() {

}

void Renderer::Render() {
  
}
