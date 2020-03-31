#ifndef KAKERA_ENGINE_TEXT_SHADER
#define KAKERA_ENGINE_TEXT_SHADER

#include "shader_base.hpp"

#define KAKERA_SHADER_TEXT TextShader::get()

class TextShader : public ShaderBase
{
private:
    TextShader()
    {
        const char* fragment = "#version 430 core\n"
            "out vec4 frag_color;"
            "in vec2 tex_coords;"
            "uniform sampler2D text;"
            "uniform vec3 attached_color;"
            "uniform float opacity = 1.0;"
            "void main() {"
            "vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, tex_coords).r);"
            "frag_color = vec4(attached_color, opacity) * sampled;}";

        compile(fragment);
    }
protected:
    ~TextShader()
    {
        if (glIsProgram(program)) glDeleteProgram(program);
    }
public:
    KAKERA_DISABLE_COPY(TextShader);
    KAKERA_DISABLE_MOVE(TextShader);

    void use() override
    {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        ShaderBase::use();
    }

    void attach_color(SDL_Color color) override
    {
        int attached_color_localtion = glGetUniformLocation(program, "attached_color");
        glUniform3f(attached_color_localtion, static_cast<float>(color.r) / 255.0f, static_cast<float>(color.g) / 255.0f, static_cast<float>(color.b) / 255.0f);
    }

    static TextShader& get()
    {
        static TextShader instance;
        return instance;
    }
};

#endif // !KAKERA_ENGINE_TEXT_SHADER
