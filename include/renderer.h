#ifndef RENDERER_H_
#define RENDERER_H_

#include <SDL.h>

#include "buffer.h"

class Renderer {
public:
  Renderer();

  Renderer(Renderer&& other) = default;
  Renderer& operator=(Renderer&& other) = default;

  ~Renderer();

  bool Create(SDL_Window *window);
  void Destroy();

  void Clear() const;

  void DrawArrays(GLenum mode, const Buffer &buffer) const;
  void DrawArrays(GLenum mode, GLint offset, const Buffer &buffer) const;
  void DrawElements(GLenum mode, const Buffer &buffer) const;
  void DrawElements(GLenum mode, GLint offset, const Buffer &buffer) const;

private:
  SDL_GLContext gl_context_ = NULL;
};

#endif  // RENDERER_H_
