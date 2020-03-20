#ifdef _WIN32
#   define WIN32_LEAN_AND_MEAN
#   include <Windows.h>
#endif // !_WIN32
#define SDL_MAIN_HANDLED
#include "SDL.h"
#include "SDL_image.h"
#include "glad/glad.h"
#if (!defined(_DEBUG) || defined(NDEBUG))
#   include "msgpack.hpp"
#else
#   include "pugixml.hpp"
#endif
#include "component/window.h"

void load_config(Window& window)
{
#if (!defined(_DEBUG) || defined(NDEBUG))
    constexpr const char* config_file = "config.mgp";
#else
    constexpr const char* config_file = "D:\\codes\\kakera\\config.xml";

    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(config_file);
    if (result.status != pugi::status_ok) return;
    pugi::xml_node root = doc.child("KakeraConfig");

    pugi::xml_node window_configs = root.child("Window");

    int width = window_configs.child("Size").attribute("width").as_int();
    int height = window_configs.child("Size").attribute("height").as_int();
    window.set_size(width, height);

    std::string title = window_configs.child_value("Title");
    window.set_title(title);

    std::string icon = window_configs.child_value("Icon");
    if (icon != "none") {
        SDL_Surface* icon_surface = IMG_Load(icon.c_str());
        window.set_icon(icon_surface);
        SDL_FreeSurface(icon_surface);
    }
#endif
}

int main(int argc, char* argv[])
{
    // Init SDL
    SDL_SetMainReady();
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetSwapInterval(1);

    IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);

    // Windows HiDPI
#ifdef _WIN32
    typedef HRESULT(*WINAPI_SETDPI)(int);
    HMODULE shcore_dll = LoadLibrary("Shcore.dll");
    if (shcore_dll != NULL) {
        WINAPI_SETDPI setdpi_func = (WINAPI_SETDPI)GetProcAddress(shcore_dll, "SetProcessDpiAwareness");
        setdpi_func(1);
        FreeLibrary(shcore_dll);
    }
#endif // _WIN32

    // Create main window.
    Window main_window;

    // Init OpenGL
    gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Load config file
    load_config(main_window);

    // Run
    main_window.run();

    // Clean up
    IMG_Quit();
    SDL_Quit();
    return 0;
}

#if (defined(_WIN32) && (!defined(_DEBUG) || defined(NDEBUG)))
extern int __argc;
extern char** __argv;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, LPSTR cmdLine, int showCmd)
{
    return main(__argc, __argv);
}
#endif // !defined(_DEBUG) && defined(_WIN32)
