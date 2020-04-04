#ifndef KAKERA_ENGINE_COMPONENT_COLOR_PICKER
#define KAKERA_ENGINE_COMPONENT_COLOR_PICKER

#include "component_base.hpp"
#include "../graphic/render_object.hpp"
#include "../graphic/shader_color_picker_circle.hpp"
#include "../graphic/shader_color_picker_triangle.hpp"
#include <memory>
#include <functional>
#include <cmath>

// ColorPicker type id = 5

class ColorPicker : public Component
{
private:
    std::unique_ptr<RenderObject> circle;
    std::unique_ptr<RenderObjectBase<3>> triangle;
public:
    KAKERA_DISABLE_COPY(ColorPicker);

    ColorPicker()
    {
        circle = std::make_unique<RenderObject>();
        triangle = std::make_unique<RenderObjectBase<3>>();
        circle->set_shader(&KAKERA_SHADER_COLOR_PICKER_CIRCLE);
        triangle->set_shader(&KAKERA_SHADER_COLOR_PICKER_TRIANGLE);
    }

    ColorPicker(ColorPicker&& other) noexcept : Component::Component(std::move(other))
    {
        circle = std::move(other.circle);
        triangle = std::move(other.triangle);
    }

    ~ColorPicker() = default;

    void set_size(int width, int height) override
    {
        Component::set_size(width, height);
        circle->set_size(width, width);
        triangle->set_size(width - 170, width - 170);
    }

    void set_position(int x, int y) override
    {
        Component::set_position(x, y);
        circle->set_position(x, y);
        triangle->set_position(x + 80, y + 40);
    }

    void render() override
    {
        circle->render([this] {
            KAKERA_SHADER_COLOR_PICKER_CIRCLE.set_radius(width / 2);
            KAKERA_SHADER_COLOR_PICKER_CIRCLE.set_circle_width(width / 10);
            int gl_y = KAKERA_SHADER_COLOR_PICKER_CIRCLE.get_window_size().second - y;
            KAKERA_SHADER_COLOR_PICKER_CIRCLE.set_center_point(x + width / 2, gl_y - width / 2);
        });

        triangle->render([this] { int a = 0; });
    }
};

#endif // !KAKERA_ENGINE_COMPONENT_COLOR_PICKER
