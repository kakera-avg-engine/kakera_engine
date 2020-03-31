#ifndef KAKERA_ENGINE_NORMAL_SHADER
#define KAKERA_ENGINE_NORMAL_SHADER

#include "shader_base.hpp"

#define KAKERA_SHADER_NORMAL NormalShader::get()

class NormalShader : public ShaderBase
{
private:
    NormalShader()
    {
        const char* fragment = "#version 430 core\n"
            "out vec4 frag_color;"
            "in vec2 tex_coords;"
            "uniform sampler2D image;"
            "uniform vec3 attached_color;"
            "uniform bool is_attach = false;"
            "uniform float opacity = 1.0;"
            "void main() {"
            "if (is_attach) {"
            "frag_color = vec4(attached_color, opacity) * texture2D(image, tex_coords);"
            "} else {"
            "frag_color = vec4(1.0, 1.0, 1.0, opacity) * texture2D(image, tex_coords);}}";

        compile(fragment);
    }
protected:
    ~NormalShader()
    {
        if (glIsProgram(program)) glDeleteProgram(program);
    }
public:
    KAKERA_DISABLE_COPY(NormalShader);
    KAKERA_DISABLE_MOVE(NormalShader);

    void use() override
    {
        glPixelStorei(GL_PACK_ALIGNMENT, 1);
        ShaderBase::use();
    }

    static NormalShader& get()
    {
        static NormalShader instance;
        return instance;
    }
};

#endif // !KAKERA_ENGINE_NORMAL_SHADER
