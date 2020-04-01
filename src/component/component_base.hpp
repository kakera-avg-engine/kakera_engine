#ifndef KAKERA_ENGINE_COMPONENT_BASE
#define KAKERA_ENGINE_COMPONENT_BASE

#include "../graphic/render_object.h"
#include "../graphic/texture.h"
#include "component_event_handler.h"
#include "../copy_and_move.inc"
#include <list>
#include <utility>
#include <string>
#include <memory>

class Component
{
protected:
    int width = 0, height = 0;
    int x = 0, y = 0;
    std::string uuid;

    bool mouse_entered = false;
public:
    Component* parent = nullptr;
    std::list<std::unique_ptr<Component>> children;

    Component() = default;

    Component(Component&& other) noexcept
    {
        width = other.width;
        height = other.height;
        x = other.x;
        y = other.y;
        uuid = other.uuid;
        mouse_entered = other.mouse_entered;

        other.width = 0;
        other.height = 0;
        other.x = 0;
        other.y = 0;
        other.uuid = "";
        other.mouse_entered = false;
    }

    Component& operator=(Component&& other) noexcept
    {
        if (&other != this) {
            width = other.width;
            height = other.height;
            x = other.x;
            y = other.y;
            uuid = other.uuid;
            mouse_entered = other.mouse_entered;

            other.width = 0;
            other.height = 0;
            other.x = 0;
            other.y = 0;
            other.uuid = "";
            other.mouse_entered = false;
        }

        return *this;
    }

    bool is(std::string uuid)
    {
        return uuid == this->uuid;
    }

    void set_uuid(std::string uuid)
    {
        this->uuid = uuid;
    }

    std::pair<int, int> get_size()
    {
        return std::make_pair(width, height);
    }

    virtual void set_size(int width, int height)
    {
        this->width = width;
        this->height = height;
    }

    std::pair<int, int> get_position()
    {
        return std::make_pair(x, y);
    }

    virtual void set_position(int x, int y)
    {
        this->x = x;
        this->y = y;
    }

    virtual bool is_coord_hit(int x, int y)
    {
        return (x >= this->x) && 
               (x <= this->x + this->width) &&
               (y >= this->y) &&
               (y <= this->y + this->height);
    }

    virtual ComponentMouseState is_mouse_enter(int x, int y)
    {
        if (is_coord_hit(x, y) && !mouse_entered) {
            mouse_entered = true;
            return ComponentMouseState::Entered;
        }
        else if (!is_coord_hit(x, y) && mouse_entered) {
            mouse_entered = false;
            return ComponentMouseState::Leave;
        }
        else {
            return ComponentMouseState::None;
        }
    }

    virtual ~Component() {};
    virtual void render() {};

    virtual void on_mouse_enter(ComponentEventHandler event) {}
    virtual void on_mouse_leave(ComponentEventHandler event) {}
    virtual void on_mouse_down(ComponentEventHandler event) {}
    virtual void on_mouse_up(ComponentEventHandler event) {}
    virtual void on_click(ComponentEventHandler event) {}
    virtual void on_double_click(ComponentEventHandler event) {}
    virtual void on_mouse_wheel_rotate(ComponentEventHandler event) {}
};

// Root type id = 0

using Root = Component;

#endif // !KAKERA_ENGINE_COMPONENT_BASE
