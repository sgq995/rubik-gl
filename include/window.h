#ifndef WINDOW_H_
#define WINDOW_H_

#include <string>

#include <SDL.h>

#include "renderer.h"

class Window {
public:
    struct Properties {
        std::string title;
        int x;
        int y;
        int width;
        int height;
    };

    Window();
    Window(const Window::Properties &properties);
    
    Window(Window&& other) = default;
    Window& operator=(Window&& other) = default;
    
    ~Window();

    bool Init(const Window::Properties &properties);

    void Clear();
    void Swap();

    const Renderer& renderer() const;
    // void set_renderer(Renderer&& renderer);

private:
    SDL_Window *window_ = NULL;

    Renderer renderer_;
};

#endif  // WINDOW_H_
