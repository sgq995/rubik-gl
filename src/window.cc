#include "window.h"

Window::Window(const Window::Properties &properties) {
  SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

  window_ = SDL_CreateWindow("RubikGL",
      SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
      800, 600, 
      SDL_WINDOW_OPENGL);

  renderer_.Create(window_);
}

Window::~Window() {
  renderer_.Destroy();

  SDL_DestroyWindow(window_);
}


void Window::Render() {
  renderer_.Clear();
  renderer_.Render();

  SDL_GL_SwapWindow(window_);
}


const Renderer& Window::renderer() const {
  return renderer_;
}
