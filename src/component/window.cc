#include "window.h"

void Window::resize_setting(int w, int h)
{
    for (auto& page : pages) {
        page.second->resize(w, h);
    }
    glViewport(0, 0, w, h);
    KAKERA_SHADER_NORMAL.set_projection(w, h);
    KAKERA_SHADER_TEXT.set_projection(w, h);
}

void Window::event_executor(SDL_Event& event)
{
    switch (event.type)
    {
    /* Window events */
    case SDL_WINDOWEVENT:
    {
        switch (event.window.type)
        {
        case SDL_WINDOWEVENT_SIZE_CHANGED:
        {
            resize_setting(event.window.data1, event.window.data2);
            break;
        }
        }
        break;
    }
    /* Mouse events */
    case SDL_MOUSEMOTION:
    {
        int x, y;
        Uint32 button = SDL_GetMouseState(&x, &y);
        auto page = active_pages.back();
        Component* com = nullptr;
        MouseState state = MouseState::None;
        for (auto iter = page->component_tree_cache.rbegin();
            iter != page->component_tree_cache.rend(); iter++) {
            state = (*iter)->is_mouse_enter(event.button.x, event.button.y);
            if (state != MouseState::None) {
                com = (*iter);
                break;
            }
        }

        MouseMotionEventHandler handler = { page, x, y };

        if (state == MouseState::Entered && com != nullptr) {
            while (com->parent) {
                com->on_mouse_enter(handler);
                com = com->parent;
            }
        }
        else if (state == MouseState::Leave && com != nullptr){
            while (com->parent) {
                com->on_mouse_leave(handler);
                com = com->parent;
            }
        }
        break;
    }
    case SDL_MOUSEBUTTONDOWN:
    {
        auto page = active_pages.back();
        Component* com = nullptr;
        for (auto iter = page->component_tree_cache.rbegin();
            iter != page->component_tree_cache.rend(); iter++) {
            if ((*iter)->is_coord_hit(event.button.x, event.button.y)) {
                com = (*iter);
                break;
            }
        }

        MouseButton button;
        switch (event.button.button)
        {
        case SDL_BUTTON_LEFT:
            button = MouseButton::Left;
            break;
        case SDL_BUTTON_RIGHT:
            button = MouseButton::Right;
            break;
        case SDL_BUTTON_MIDDLE:
            button = MouseButton::Middle;
            break;
        case SDL_BUTTON_X1:
            button = MouseButton::Custom_1;
            break;
        case SDL_BUTTON_X2:
            button = MouseButton::Custom_2;
            break;
        default:
            break;
        }

        MouseButtonEventHandler handler = { page, event.button.x, event.button.y, button };

        if (com != nullptr) {
            while (com->parent) {
                com->on_mouse_down(handler);
                com = com->parent;
            }
        }
        break;
    }
    case SDL_MOUSEBUTTONUP:
    {
        auto page = active_pages.back();
        Component* com = nullptr;
        for (auto iter = page->component_tree_cache.rbegin();
            iter != page->component_tree_cache.rend(); iter++) {
            if ((*iter)->is_coord_hit(event.button.x, event.button.y)) {
                com = (*iter);
                break;
            }
        }


        MouseButton button;
        switch (event.button.button)
        {
        case SDL_BUTTON_LEFT:
            button = MouseButton::Left;
            break;
        case SDL_BUTTON_RIGHT:
            button = MouseButton::Right;
            break;
        case SDL_BUTTON_MIDDLE:
            button = MouseButton::Middle;
            break;
        case SDL_BUTTON_X1:
            button = MouseButton::Custom_1;
            break;
        case SDL_BUTTON_X2:
            button = MouseButton::Custom_2;
            break;
        default:
            break;
        }

        MouseButtonEventHandler handler = { page, event.button.x, event.button.y, button };

        if (com != nullptr) {
            while (com->parent) {
                com->on_mouse_up(handler);

                if (event.button.clicks == 1)
                    com->on_click(handler);
                else if (event.button.clicks == 2)
                    com->on_double_click(handler);

                com = com->parent;
            }
        }
        break;
    }
    case SDL_MOUSEWHEEL:
    {
        int x, y;
        SDL_GetMouseState(&x, &y);
        auto page = active_pages.back();
        Component* com = nullptr;
        for (auto iter = page->component_tree_cache.rbegin();
            iter != page->component_tree_cache.rend(); iter++) {
            if ((*iter)->is_coord_hit(x, y)) {
                com = (*iter);
                break;
            }
        }

        if (event.wheel.direction == SDL_MOUSEWHEEL_FLIPPED)
            event.wheel.y *= -1;

        MouseWheelEventHandler handler;
        handler.page = page;
        handler.distance = std::abs(event.wheel.y);
        if (event.wheel.y > 0)
            handler.direction = MouseWheelDirection::Up;
        else
            handler.direction = MouseWheelDirection::Down;

        if (com != nullptr) {
            while (com->parent) {
                com->on_mouse_wheel_rotate(handler);
                com = com->parent;
            }
        }
        break;
    }
    /* Keyboard events */
    case SDL_KEYDOWN:
    {
        auto page = active_pages.back();
        KeyEventHandler handler = { page, event.key.keysym.scancode };
        for (auto& com : page->component_tree_cache) {
            if (com->parent)
                com->on_key_down(handler);
        }
        break;
    }
    case SDL_KEYUP:
    {
        auto page = active_pages.back();
        KeyEventHandler handler = { page, event.key.keysym.scancode };
        for (auto& com : page->component_tree_cache) {
            if (com->parent)
                com->on_key_up(handler);
        }
        break;
    }
    default:
        break;
    }
}

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

    if (page->get_display_mode() == PageDisplayMode::Unique && !active_pages.empty()) {
        for (auto a_page : active_pages)
            a_page->on_suspend();

        active_pages.clear();
    }

    active_pages.push_back(page);
    page->on_active();
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
        resize_setting(w, h);
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
            event_executor(event_queue);
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
