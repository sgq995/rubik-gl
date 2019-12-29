#include <iostream>

#include "SDL.h"

#include "config.h"

#define PRINT_ERROR_MESSAGE(msg) \
    std::cout << msg << " failed: " << SDL_GetError() << std::endl

enum return_code {
  OK,
  ERR_SDL_INIT,
  ERR_SDL_CREATE_WINDOW,
  ERR_SDL_CREATE_RENDERER,
};

int main(int argc, char **argv) {
  int status = SDL_Init(SDL_INIT_VIDEO);
  if (status < 0) {
    PRINT_ERROR_MESSAGE("SDL_Init");
    return ERR_SDL_INIT;
  }

  SDL_Window *window = SDL_CreateWindow("RubikGL",
      SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
      800, 600, 
      SDL_WINDOW_OPENGL);
  if (window == NULL) {
    PRINT_ERROR_MESSAGE("SDL_CreateWindow");
    return ERR_SDL_CREATE_WINDOW;
  }

  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if (renderer == NULL) {
    PRINT_ERROR_MESSAGE("SDL_CreateRenderer");
    return ERR_SDL_CREATE_RENDERER;
  }

  SDL_Event event;

  while (true) {
    if (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        break;
      }
    }

    SDL_RenderClear(renderer);

    SDL_RenderPresent(renderer);
  }

  SDL_DestroyWindow(window);

  SDL_Quit();

  return OK;
}
