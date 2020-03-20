#ifndef KAKERA_ENGINE_COMPONENT_WINDOW
#define KAKERA_ENGINE_COMPONENT_WINDOW

#include "SDL.h"
#include <utility>
#include <string>
#include <list>
#include <memory>
#include <stack>
#include "page.h"
#include "../copy_and_move.inc"

class Window
{
private:
    SDL_Window* window_ptr = nullptr;
    SDL_GLContext gl_context = nullptr;

    std::list<std::unique_ptr<Page>> pages;
    std::stack<Page*> active_pages;
public:
    Window();
    ~Window();

    KAKERA_DISABLE_COPY(Window);
    KAKERA_DISABLE_MOVE(Window);

    std::pair<int, int> get_size();
    void set_size(int width, int height);

    std::string get_title();
    void set_title(std::string title);

    void set_icon(SDL_Surface* icon_surface);

    void fullscreen();
    void windowed();

    void run();
};

#endif // !KAKERA_ENGINE_COMPONENT_WINDOW