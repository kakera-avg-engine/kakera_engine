#ifndef KAKERA_ENGINE_COMPONENT_WINDOW
#define KAKERA_ENGINE_COMPONENT_WINDOW

#include "SDL.h"
#include <utility>
#include <string>
#include "page.h"
#include "../disable_copy_and_move.inc"

class Window
{
private:
    SDL_Window* window_ptr = nullptr;
public:
    Window();
    ~Window();

    KAKERA_DISABLE_COPY(Window);
    KAKERA_DISABLE_MOVE(Window);

    std::pair<int, int> get_size();
    void set_size(int width, int height);

    std::string get_title();
    void set_title(std::string title);

    bool is_set_icon();
    void set_icon(SDL_Surface* icon_surface);

    void show();
    void hide();
    void fullscreen();
    void windowed();

    void run();
};

#endif // !KAKERA_ENGINE_COMPONENT_WINDOW