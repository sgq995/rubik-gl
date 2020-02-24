#include "window.h"

#include <iostream>

Window::Window() {

}

Window::Window(const Window::Properties &properties) {
  Create(properties);
}

Window::~Window() {
  renderer_.Destroy();

  SDL_DestroyWindow(window_);
}


bool Window::Create(const Window::Properties &properties) {
  SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

  window_ = SDL_CreateWindow(properties.title.c_str(),
      properties.x, properties.y,
      properties.width, properties.height, 
      SDL_WINDOW_OPENGL);
  if (window_ == NULL) {
    return window_;
  }

  return renderer_.Create(window_);
}


void Window::Clear() {
  renderer_.Clear();
}

void Window::Swap() {
  SDL_GL_SwapWindow(window_);
}


const Renderer& Window::renderer() const {
  return renderer_;
}
