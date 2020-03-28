#ifdef _WIN32
#   define WIN32_LEAN_AND_MEAN
#   include <Windows.h>
#endif // !_WIN32

#define SDL_MAIN_HANDLED
#include "SDL.h"
#include "SDL_image.h"

#include "glad/glad.h"

extern "C"
{
#include "libavformat/avformat.h"
}

#if (!defined(_DEBUG) || defined(NDEBUG))
#   include "msgpack.hpp"
#else
#   include "pugixml.hpp"
#endif

#include "component/window.h"
#include "log/log.h"
#include "file_package/package_manager.h"

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

    std::string debug_root_dir = root.child_value("DebugRootDir");
    debug_root_dir += "/";

    pugi::xml_node window_configs = root.child("Window");

    int width = window_configs.child("Size").attribute("width").as_int();
    int height = window_configs.child("Size").attribute("height").as_int();
    window.set_size(width, height);

    std::string title = window_configs.child_value("Title");
    window.set_title(title);

    std::string icon = window_configs.child_value("Icon");
    if (icon != "none") {
        icon = debug_root_dir + icon;
        SDL_Surface* icon_surface = IMG_Load(icon.c_str());
        window.set_icon(icon_surface);
        SDL_FreeSurface(icon_surface);
    }

    pugi::xml_node packages = root.child("Packages");
    for (auto& package : packages) {
        std::string id = package.attribute("id").as_string();
        std::string src = package.attribute("src").as_string();
        src = debug_root_dir + src;
        KAKERA_PACKAGE_MANAGER.add_package(id, Package(src));
    }

    pugi::xml_node pages = root.child("Pages");
    for (auto& page : pages) {
        std::string id = page.attribute("id").as_string();
        std::string src = page.attribute("src").as_string();
        src = debug_root_dir + src;
        window.pages.try_emplace(id, std::make_unique<Page>(src));
        if (page.attribute("default").as_bool()) window.start_page(id);
    }
#endif
}

int main(int argc, char* argv[])
{
    // Init SDL
    SDL_SetMainReady();
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) != 0) {
        KAKERA_LOG(LogLevel::Fatal, "Can't load SDL2.");
    }
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);

    // Init FFmpeg
    av_register_all();

    // Windows HiDPI
#ifdef _WIN32
    typedef HRESULT(*WINAPI_SETDPI)(int);
    HMODULE shcore_dll = LoadLibrary("Shcore.dll");
    if (shcore_dll != NULL) {
        WINAPI_SETDPI setdpi_func = (WINAPI_SETDPI)GetProcAddress(shcore_dll, "SetProcessDpiAwareness");
        setdpi_func(1);
        FreeLibrary(shcore_dll);
    }
    else {
        KAKERA_LOG(LogLevel::Warning, "Too old Windows version to enable HiDPI.");
    }
#endif // _WIN32

    // Create main window.
    Window main_window;

    // Init OpenGL
    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
        KAKERA_LOG(LogLevel::Fatal, "Can't load OpenGL library.");
    }
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Load config file
    load_config(main_window);

    // Run
    main_window.run();

    // Clean up
    main_window.quit();
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
