#ifndef KAKERA_ENGINE_GRAPHIC_SHADER_BASE
#define KAKERA_ENGINE_GRAPHIC_SHADER_BASE

#include "SDL.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glad/glad.h"
#include "../copy_and_move.inc"

class ShaderBase
{
protected:
    glm::mat4 projection;
    GLuint program;

    void compile(const char* frag_code)
    {
        const char* vertex_code = "#version 430 core\n" \
            "layout (location = 0) in vec2 vertex;" \
            "layout (location = 1) in vec2 tex_coord;" \
            "out vec2 tex_coords;" \
            "uniform mat4 model;" \
            "uniform mat4 projection;" \
            "void main() {" \
            "tex_coords = tex_coord;" \
            "gl_Position = projection * model * vec4(vertex, 0.0, 1.0);}";

        unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
        unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

        glShaderSource(vertex_shader, 1, &vertex_code, NULL);
        glShaderSource(fragment_shader, 1, &frag_code, NULL);

        glCompileShader(vertex_shader);
        glCompileShader(fragment_shader);

        program = glCreateProgram();
        glAttachShader(program, vertex_shader);
        glAttachShader(program, fragment_shader);
        glLinkProgram(program);

        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);
    }

    virtual ~ShaderBase() {};
public:
    virtual void use()
    {
        glUseProgram(program);
    }

    virtual void attach_color(SDL_Color color)
    {
        int attached_color_localtion = glGetUniformLocation(program, "attached_color");
        glUniform3f(attached_color_localtion, static_cast<float>(color.r) / 255.0f, static_cast<float>(color.g) / 255.0f, static_cast<float>(color.b) / 255.0f);

        int is_attach_location = glGetUniformLocation(program, "is_attach");
        glUniform1i(is_attach_location, GL_TRUE);
    }

    virtual void set_opacity(uint8_t opacity)
    {
        int opacity_location = glGetUniformLocation(program, "opacity");
        glUniform1f(opacity_location, static_cast<float>(opacity) / 255.0f);
    }

    virtual void set_projection(int width, int height)
    {
        projection = glm::ortho(0.0f, static_cast<float>(width), static_cast<float>(height), 0.0f, -1.0f, 1.0f);
    }

    virtual void set_model(glm::mat4 model)
    {
        int projection_location = glGetUniformLocation(program, "projection");
        glUniformMatrix4fv(projection_location, 1, GL_FALSE, glm::value_ptr(projection));
        int model_location = glGetUniformLocation(program, "model");
        glUniformMatrix4fv(model_location, 1, GL_FALSE, glm::value_ptr(model));
    }
};

#endif // !KAKERA_ENGINE_GRAPHIC_SHADER_BASE
