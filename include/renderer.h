#ifndef RENDERER_H_
#define RENDERER_H_

#include <SDL.h>

#include "shader.h"

class Renderer {
public:
  Renderer();

  Renderer(Renderer&& other) = default;
  Renderer& operator=(Renderer&& other) = default;

  ~Renderer();

  bool Create(SDL_Window *window);
  void Destroy();

  void Clear();
  void Render();

private:
  SDL_GLContext gl_context_ = NULL;

  Shader *shader_ = nullptr;
};

#endif  // RENDERER_H_
