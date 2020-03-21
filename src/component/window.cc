#include "window.h"

Window::Window()
{
    window_ptr = SDL_CreateWindow("", 0, 0, 0, 0, SDL_WINDOW_HIDDEN | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_OPENGL);
    gl_context = SDL_GL_CreateContext(window_ptr);
    SDL_GL_MakeCurrent(window_ptr, gl_context);
    fps_sem = SDL_CreateSemaphore(1);
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
    title += u8" - [KAKERA DEBUG MODE]";
#endif
    SDL_SetWindowTitle(window_ptr, title.c_str());
}

void Window::set_icon(SDL_Surface* icon_surface)
{
    SDL_SetWindowIcon(window_ptr, icon_surface);
}

void Window::start_page(std::string id)
{
    Page* page = nullptr;
    page = pages[id].get();

    if (!page) return;

    if (page->get_display_mode() == PageDisplayMode::Unique && active_pages.empty()) {
        active_pages.clear();
    }

    active_pages.push_back(page);
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
    {
        int w, h;
        SDL_GetWindowSize(window_ptr, &w, &h);
        for (auto& page : pages) {
            page.second->resize(w, h);
        }
        glViewport(0, 0, w, h);
        KAKERA_SHADER_NORMAL.set_projection(w, h);
    }

    SDL_SetWindowPosition(window_ptr, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
    SDL_ShowWindow(window_ptr);

    bool quit = false;
    SDL_Event event_queue;

    while (!quit) {
        SDL_TimerID fps_timer = SDL_AddTimer(16, [](Uint32 interval, void* arg) -> Uint32 {
            SDL_sem* fps_sem = static_cast<SDL_sem*>(arg);
            SDL_SemPost(fps_sem);
            return interval;
        }, fps_sem);

        while (SDL_PollEvent(&event_queue)) {
            if (event_queue.type == SDL_QUIT) {
                quit = true;
            }
        }

        for (auto page : active_pages) page->draw();

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        for (auto page : active_pages) page->render();

        SDL_GL_SwapWindow(window_ptr);

        SDL_SemWait(fps_sem);
        SDL_RemoveTimer(fps_timer);
    }
}

void Window::quit()
{
    SDL_DestroySemaphore(fps_sem);
    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window_ptr);
}
