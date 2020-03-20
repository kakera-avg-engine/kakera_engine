#include "window.h"

Window::Window()
{
    window_ptr = SDL_CreateWindow("", 0, 0, 0, 0, SDL_WINDOW_HIDDEN | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_OPENGL);
    gl_context = SDL_GL_CreateContext(window_ptr);
    SDL_GL_MakeCurrent(window_ptr, gl_context);
}

Window::~Window()
{
    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window_ptr);
}

std::pair<int, int> Window::get_size()
{
    int w, h;
    SDL_GetWindowSize(window_ptr, &w, &h);
    return std::make_pair(w, h);
}

void Window::set_size(int width, int height)
{
    SDL_SetWindowSize(window_ptr, width, height);
}

std::string Window::get_title()
{
    return std::string(SDL_GetWindowTitle(window_ptr));
}

void Window::set_title(std::string title)
{
#if (defined(_DEBUG) || !defined(NDEBUG))
    title += " - [KAKERA DEBUG MODE]";
#endif
    SDL_SetWindowTitle(window_ptr, title.c_str());
}

void Window::set_icon(SDL_Surface* icon_surface)
{
    SDL_SetWindowIcon(window_ptr, icon_surface);
}

void Window::fullscreen()
{
    SDL_SetWindowFullscreen(window_ptr, SDL_WINDOW_FULLSCREEN_DESKTOP);
}

void Window::windowed()
{
    SDL_SetWindowFullscreen(window_ptr, 0);
}

void Window::run()
{
    SDL_SetWindowPosition(window_ptr, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

    SDL_ShowWindow(window_ptr);

    bool quit = false;
    SDL_Event event_queue;

    while (!quit) {
        while (SDL_PollEvent(&event_queue)) {
            if (event_queue.type == SDL_QUIT) {
                quit = true;
            }
        }

        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        SDL_GL_SwapWindow(window_ptr);
    }
}
