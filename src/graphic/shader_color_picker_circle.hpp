#ifndef KAKERA_ENGINE_GRAPHIC_COLOR_PICKER_CIRCLE_SHADER
#define KAKERA_ENGINE_GRAPHIC_COLOR_PICKER_CIRCLE_SHADER

#include "shader_base.hpp"

#define KAKERA_SHADER_COLOR_PICKER_CIRCLE ColorPickerCircleShader::get()

class ColorPickerCircleShader : public ShaderBase
{
private:
    static constexpr char* vertex = "#version 430 core\n"
        "layout (location = 0) in vec2 vertex;"
        "uniform mat4 model;"
        "uniform mat4 projection;"
        "void main() {"
        "gl_Position = projection * model * vec4(vertex, 0.0, 1.0);}";

    static constexpr char* fragment = "#version 430 core\n"
        "#define TWO_PI -6.28318530718\n"
        "out vec4 frag_color;"
        "layout(pixel_center_integer) in vec4 gl_FragCoord;"
        "uniform int radius;"
        "uniform vec2 center_point;"
        "uniform int circle_width;"
        "uniform float opacity = 1.0;"
        "vec3 hsv2rgb(vec3 c) {"
        "vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);"
        "vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);"
        "return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);}"
        "void main() {"
        "float dis_center = distance(gl_FragCoord.xy, center_point);"
        "vec2 pos = gl_FragCoord.xy - center_point;"
        "float angle = atan(pos.y, pos.x);"
        "if (dis_center <= radius && dis_center >= (radius - circle_width)) {"
        "vec3 color = hsv2rgb(vec3((angle / TWO_PI) + 0.25, 1.0, 1.0));"
        "frag_color = vec4(color, 1.0);"
        "} else {"
        "frag_color = vec4(0.0, 0.0, 0.0, 0.0);}}";

    ColorPickerCircleShader()
    {
        compile(fragment, vertex);
    }
public:
    KAKERA_DISABLE_COPY(ColorPickerCircleShader);
    KAKERA_DISABLE_MOVE(ColorPickerCircleShader);

    void set_radius(int radius)
    {
        int radius_location = glGetUniformLocation(program, "radius");
        glUniform1i(radius_location, radius);
    }

    void set_center_point(float x, float y)
    {
        int center_point_location = glGetUniformLocation(program, "center_point");
        glUniform2f(center_point_location, x, y);
    }

    void set_circle_width(int width)
    {
        int circle_width_location = glGetUniformLocation(program, "circle_width");
        glUniform1i(circle_width_location, width);
    }

    static ColorPickerCircleShader& get()
    {
        static ColorPickerCircleShader instance;
        return instance;
    }
};

#endif // !KAKERA_ENGINE_GRAPHIC_COLOR_PICKER_CIRCLE_SHADER
