#ifndef KAKERA_ENGINE_COMPONENT_WINDOW
#define KAKERA_ENGINE_COMPONENT_WINDOW

#include "SDL.h"
#include <utility>
#include <string>
#include <unordered_map>
#include <memory>
#include <vector>
#include "page.h"
#include "../graphic/shader_normal.hpp"
#include "../graphic/shader_text.hpp"
#include "../copy_and_move.inc"

class Window
{
private:
    SDL_Window* window_ptr = nullptr;
    SDL_GLContext gl_context = nullptr;
    SDL_sem* fps_sem = nullptr;

    std::vector<Page*> active_pages;
public:
    std::unordered_map<std::string, std::unique_ptr<Page>> pages;

    Window();

    KAKERA_DISABLE_COPY(Window);
    KAKERA_DISABLE_MOVE(Window);

    std::pair<int, int> get_size();
    void set_size(int width, int height);

    std::string get_title();
    void set_title(std::string title);

    void set_icon(SDL_Surface* icon_surface);

    void start_page(std::string id);

    void fullscreen();
    void windowed();

    void run();
    void quit();
};

#endif // !KAKERA_ENGINE_COMPONENT_WINDOW