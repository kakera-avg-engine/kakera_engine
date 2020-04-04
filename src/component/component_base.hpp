#ifndef KAKERA_ENGINE_COMPONENT_BASE
#define KAKERA_ENGINE_COMPONENT_BASE

#include "../graphic/render_object.hpp"
#include "../graphic/texture.h"
#include "component_event_handler.h"
#include "../cubic_bezier.hpp"
#include "../copy_and_move.inc"
#include <list>
#include <utility>
#include <string>
#include <memory>

#include "../log/log.h"

struct AnimateTarget
{
    int w = -1, h = -1;
    int x = -1, y = -1;
    float angle = -1.0f;
    uint8_t opacity = -1;
};

class Component
{
protected:
    int width = 0, height = 0;
    int x = 0, y = 0;
    float rotation_angle = 0.0f;
    uint8_t opacity = 255;
    std::string uuid;
    bool is_hidden = false;

    AnimateTarget animation_target;
    int animation_steps = 0;
    int animation_index = -1;
    CubicBezier animation_speed;

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
        rotation_angle = other.rotation_angle;
        opacity = other.opacity;
        is_hidden = other.is_hidden;
        mouse_entered = other.mouse_entered;
        animation_target = other.animation_target;
        animation_steps = other.animation_steps;
        uuid = std::move(other.uuid);
        animation_speed = std::move(other.animation_speed);

        other.width = 0;
        other.height = 0;
        other.x = 0;
        other.y = 0;
        other.rotation_angle = 0.0f;
        other.opacity = 255;
        other.is_hidden = false;
        other.mouse_entered = false;
        other.animation_target = AnimateTarget();
        other.animation_steps = 0;
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

    virtual void set_rotation_angle(float angle)
    {
        if (angle >= 360.0f) {
            int a = angle / 360.0f;
            angle -= (360.0f * a);
        }
        this->rotation_angle = angle;
    }

    float get_rotation_angle()
    {
        return rotation_angle;
    }

    virtual void set_opacity(uint8_t opacity)
    {
        this->opacity = opacity;
    }

    uint8_t get_opacity()
    {
        return opacity;
    }

    void hidden()
    {
        this->is_hidden = true;
    }

    void display()
    {
        this->is_hidden = false;
    }

    virtual void set_animate(AnimateTarget target, int time, CubicBezier speed)
    {
        animation_target = target;
        animation_steps = time / 16;
        animation_speed = speed;
        animation_speed.set_steps(animation_steps);
        animation_steps -= 1;
        animation_index = 0;
    }

    void play_animate()
    {
        if (animation_index == -1 || animation_index > animation_steps)
            return;

        float d = animation_speed.results[animation_index].y;

        if (animation_target.x >= 0)
            x += ((animation_target.x - x) * d);
        if (animation_target.y >= 0)
            y += ((animation_target.y - y) * d);
        if (animation_target.w >= 0)
            width += ((animation_target.w - width) * d);
        if (animation_target.h >= 0)
            height += ((animation_target.h - height) * d);
        if (animation_target.angle >= 0)
            rotation_angle += ((animation_target.angle - rotation_angle) * d);
        if (animation_target.opacity >= 0)
            opacity += ((animation_target.opacity - opacity) * d);

        animation_index++;
    }

    void reset_animate()
    {
        animation_index = 0;
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

    MouseState is_mouse_enter(int x, int y)
    {
        if (is_coord_hit(x, y) && !mouse_entered) {
            mouse_entered = true;
            return MouseState::Entered;
        }
        else if (!is_coord_hit(x, y) && mouse_entered) {
            mouse_entered = false;
            return MouseState::Leave;
        }
        else {
            return MouseState::None;
        }
    }

    virtual ~Component() {};
    virtual void render() {};

    virtual void on_mouse_enter(EventHandler event) {}
    virtual void on_mouse_leave(EventHandler event) {}
    virtual void on_mouse_down(EventHandler event) {}
    virtual void on_mouse_up(EventHandler event) {}
    virtual void on_click(EventHandler event) {}
    virtual void on_double_click(EventHandler event) {}
    virtual void on_mouse_wheel_rotate(EventHandler event) {}
    virtual void on_key_down(EventHandler event) {}
    virtual void on_key_up(EventHandler event) {}
};

// Root type id = 0

using Root = Component;

#endif // !KAKERA_ENGINE_COMPONENT_BASE
