#ifndef KAKERA_ENGINE_GRAPHIC_COLOR_PICKER_TRIANGLE_SHADER
#define KAKERA_ENGINE_GRAPHIC_COLOR_PICKER_TRIANGLE_SHADER

#include "shader_base.hpp"

#define KAKERA_SHADER_COLOR_PICKER_TRIANGLE ColorPickerTriangleShader::get()

class ColorPickerTriangleShader : public ShaderBase
{
private:
    static constexpr char* vertex = "#version 430 core\n"
        "layout (location = 0) in vec2 vertex;"
        "layout (location = 1) in vec2 sv_color;"
        "out vec3 v_color;"
        "uniform mat4 model;"
        "uniform mat4 projection;"
        "uniform float h = 0;"
        "vec3 hsv2rgb(vec3 c) {"
        "vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);"
        "vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);"
        "return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);}"
        "void main() {"
        "gl_Position = projection * model * vec4(vertex, 0.0, 1.0);"
        "v_color = hsv2rgb(vec3(h / 360.0, sv_color));}";

    static constexpr char* fragment = "#version 430 core\n"
        "in vec3 v_color;"
        "out vec4 frag_color;"
        "void main() {"
        "frag_color = vec4(v_color, 1.0);}";

    ColorPickerTriangleShader()
    {
        compile(fragment, vertex);
    }
public:
    KAKERA_DISABLE_COPY(ColorPickerTriangleShader);
    KAKERA_DISABLE_MOVE(ColorPickerTriangleShader);

    void set_color(float h_value)
    {
        int h_location = glGetUniformLocation(program, "h");
        glUniform1f(h_location, h_value);
    }

    static ColorPickerTriangleShader& get()
    {
        static ColorPickerTriangleShader instance;
        return instance;
    }
};

#endif // !KAKERA_ENGINE_GRAPHIC_COLOR_PICKER_TRIANGLE_SHADER
