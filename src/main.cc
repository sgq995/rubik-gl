#include <iostream>

#include <GL/glew.h>

#include <SDL.h>

#include "config.h"

#include "renderer.h"
#include "window.h"

#define PRINT_ERROR_MESSAGE(msg) \
    std::cerr << msg << " failed: " << SDL_GetError() << std::endl

enum return_code {
  OK,
  ERR_SDL_INIT,
  ERR_GLEW_INIT,
};

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

    SDL_Event event;

    while (true) {
      if (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
          break;
        }
      }

      window.Render();
    }
  }

  SDL_Quit();

  return OK;
}
