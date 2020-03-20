#include "pch.h"
#define SDL_MAIN_HANDLED
#include "SDL.h"
#include "SDL_image.h"

int main(int argc, char* argv[])
{
	SDL_SetMainReady();
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetSwapInterval(1);

    IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);

    IMG_Quit();
    SDL_Quit();
    return 0;
}

#if (!defined(_DEBUG) && defined(_WIN32))
extern int __argc;
extern char** __argv;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, LPSTR cmdLine, int showCmd)
{
    return main(__argc, __argv);
}
#endif // !defined(_DEBUG) && defined(_WIN32)
