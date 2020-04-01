#ifndef KAKERA_ENGINE_COMPONENT_EVENT_HANDLER
#define KAKERA_ENGINE_COMPONENT_EVENT_HANDLER

#include <SDL.h>
#include <variant>

enum class ComponentMouseState
{
    Entered,
    Leave,
    None
};

enum class ComponentMouseButton
{
    Left,
    Right,
    Middle
};

enum class ComponentMouseWheelDirection
{
    Up,
    Down
};

struct ComponentMouseMotionEventHandler
{
    void* page;
    int x, y;
};

struct ComponentMouseButtonEventHandler
{
    void* page;
    int x, y;
    ComponentMouseButton button;
};

struct ComponentMouseWheelEventHandler
{
    void* page;
    ComponentMouseWheelDirection direction;
    int distance;
};

struct ComponentKeyEventHandler
{
    void* page;
    SDL_Scancode keycode;
};

using ComponentEventHandler = std::variant<ComponentMouseMotionEventHandler,
                                           ComponentMouseButtonEventHandler,
                                           ComponentMouseWheelEventHandler,
                                           ComponentKeyEventHandler>;

#endif // !KAKERA_ENGINE_COMPONENT_EVENT_HANDLER
