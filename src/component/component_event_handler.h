#ifndef KAKERA_ENGINE_COMPONENT_EVENT_HANDLER
#define KAKERA_ENGINE_COMPONENT_EVENT_HANDLER

#include <SDL.h>
#include <variant>

enum class MouseState
{
    Entered,
    Leave,
    None
};

enum class MouseButton
{
    Left,
    Right,
    Middle,
    Custom_1,
    Custom_2
};

enum class MouseWheelDirection
{
    Up,
    Down
};

struct MouseMotionEventHandler
{
    void* page;
    int x, y;
};

struct MouseButtonEventHandler
{
    void* page;
    int x, y;
    MouseButton button;
};

struct MouseWheelEventHandler
{
    void* page;
    MouseWheelDirection direction;
    int distance;
};

struct KeyEventHandler
{
    void* page;
    SDL_Scancode keycode;
};

using EventHandler = std::variant<MouseMotionEventHandler,
                                  MouseButtonEventHandler,
                                  MouseWheelEventHandler,
                                  KeyEventHandler>;

#endif // !KAKERA_ENGINE_COMPONENT_EVENT_HANDLER
