#ifndef KAKERA_ENGINE_GRAPHIC_RENDER_OBJECT
#define KAKERA_ENGINE_GRAPHIC_RENDER_OBJECT

#include "shader_base.hpp"
#include "texture.h"
#include <list>
#include <utility>
#include <array>
#include <climits>
#include <variant>
#include <functional>
#include "glad/glad.h"
#include "SDL.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "../copy_and_move.inc"

enum class TextureApplyMethod
{
    Repeat,
    Tile
};

using GLVertices = std::array<GLfloat, 12>;

using TextureView = std::variant<GLVertices, TextureApplyMethod>;

static TextureView normal_view = GLVertices({
    0.0f, 1.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f,
    1.0f, 1.0f, 1.0f, 0.0f
});

static TextureView upside_down_view = GLVertices({
    0.0f, 0.0f, 1.0f, 1.0f,
    0.0f, 1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 1.0f, 1.0f
});

template<int Vertices>
class RenderObjectBase
{
private:
    GLuint vao = UINT_MAX, vbo = UINT_MAX;
    Texture* texture = nullptr;
    ShaderBase* shader = nullptr;

    int width = 0, height = 0;
    int x = 0, y = 0;
    float rotation_angle = 0.0f;
    uint8_t opacity = 255;
    bool is_attach_color = false;
    SDL_Color attached_color = { 0, 0, 0, 0 };

    GLVertices method_to_vertices(TextureApplyMethod method)
    {
        // TODO
        return GLVertices({
            0.0f, 1.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f,
            1.0f, 1.0f, 1.0f, 0.0f
        });
    }
public:
    RenderObjectBase()
    {
        if constexpr (Vertices == 4) {
            GLVertices vertices = {
                0.0f, 1.0f, 1.0f, 0.0f,
                0.0f, 0.0f, 0.0f, 1.0f,
                1.0f, 1.0f, 1.0f, 0.0f
            };

            glGenVertexArrays(1, &vao);
            glGenBuffers(1, &vbo);

            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 24, nullptr, GL_STATIC_DRAW);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * 12, vertices.data());

            glBindVertexArray(vao);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 2, (void*)0);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 2, (void*)(sizeof(GLfloat) * 12));

            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
        }
        else if (Vertices == 3) {
            std::array<GLfloat, 6> vertices = {
                0.0f, 1.0f,
                1.0f, 1.0f,
                0.5f, 0.0f
            };

            std::array<GLfloat, 6> colors = {
                1.0f, 0.0f,
                0.0f, 1.0f,
                1.0f, 1.0f
            };

            glGenVertexArrays(1, &vao);
            glGenBuffers(1, &vbo);

            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 12, nullptr, GL_STATIC_DRAW);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * 6, vertices.data());
            glBufferSubData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6, sizeof(GLfloat) * 6, colors.data());

            glBindVertexArray(vao);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 2, (void*)0);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 2, (void*)(sizeof(GLfloat) * 6));

            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
        }
    }

    ~RenderObjectBase()
    {
        if (glIsBuffer(vbo)) glDeleteBuffers(1, &vbo);
        if (glIsVertexArray(vao)) glDeleteVertexArrays(1, &vao);
    }

    void set_texture(Texture* texture, TextureView texture_view = normal_view)
    {
        this->texture = texture;

        if constexpr (Vertices == 4) {
            glBindBuffer(GL_ARRAY_BUFFER, vbo);

            if (auto vertices = std::get_if<GLVertices>(&texture_view))
                glBufferSubData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 12, sizeof(GLfloat) * 12, vertices->data());
            else {
                TextureApplyMethod method = std::get<TextureApplyMethod>(texture_view);
                GLVertices tex_coord = method_to_vertices(method);
                glBufferSubData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 12, sizeof(GLfloat) * 12, tex_coord.data());
            }

            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }
    }

    void set_shader(ShaderBase* shader)
    {
        this->shader = shader;
    }

    std::pair<int, int> get_size()
    {
        return std::make_pair(width, height);
    }

    void set_size(int width, int height)
    {
        this->width = width;
        this->height = height;
    }

    std::pair<int, int> get_position()
    {
        return std::make_pair(x, y);
    }

    void set_position(int x, int y)
    {
        this->x = x;
        this->y = y;
    }

    float get_rotation_angle()
    {
        return rotation_angle;
    }

    void set_rotation_angle(float angle)
    {
        rotation_angle = angle;
    }

    uint8_t get_opacity()
    {
        return opacity;
    }

    void set_opacity(uint8_t opacity)
    {
        this->opacity = opacity;
    }

    void attach_color(SDL_Color color)
    {
        if (!is_attach_color) is_attach_color = true;
        attached_color = color;
    }

    void cancel_attach_color()
    {
        if (is_attach_color) is_attach_color = false;
    }

    Texture* get_texture()
    {
        return texture;
    }

    void render()
    {
        if constexpr (Vertices == 4) {
            if (!shader)
                return;

            shader->use();

            glm::mat4 model = glm::mat4(1.0f);

            float fx = static_cast<float>(x), fy = static_cast<float>(y);
            float fw = static_cast<float>(width), fh = static_cast<float>(height);

            model = glm::translate(model, glm::vec3(fx, fy, 0.0f));

            if (rotation_angle != 0.0f) {
                model = glm::translate(model, glm::vec3(0.5f * fw, 0.5f * fh, 0.0f));
                model = glm::rotate(model, glm::radians(rotation_angle), glm::vec3(0.0f, 0.0f, 1.0f));
                model = glm::translate(model, glm::vec3(-0.5f * fw, -0.5f * fh, 0.0f));
            }

            model = glm::scale(model, glm::vec3(fw, fh, 1.0f));

            shader->set_model(model);

            if (is_attach_color)
                shader->attach_color(attached_color);

            shader->set_opacity(opacity);

            if (texture) {
                glActiveTexture(GL_TEXTURE0);
                texture->bind();
            }

            glBindVertexArray(vao);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            glBindVertexArray(0);
        }
    }

    void render(std::function<void()> custom_settings)
    {
        if constexpr (Vertices == 4) {
            if (!shader)
                return;

            shader->use();

            glm::mat4 model = glm::mat4(1.0f);

            float fx = static_cast<float>(x), fy = static_cast<float>(y);
            float fw = static_cast<float>(width), fh = static_cast<float>(height);

            model = glm::translate(model, glm::vec3(fx, fy, 0.0f));

            if (rotation_angle != 0.0f) {
                model = glm::translate(model, glm::vec3(0.5f * fw, 0.5f * fh, 0.0f));
                model = glm::rotate(model, glm::radians(rotation_angle), glm::vec3(0.0f, 0.0f, 1.0f));
                model = glm::translate(model, glm::vec3(-0.5f * fw, -0.5f * fh, 0.0f));
            }

            model = glm::scale(model, glm::vec3(fw, fh, 1.0f));

            shader->set_model(model);

            if (is_attach_color)
                shader->attach_color(attached_color);

            shader->set_opacity(opacity);

            if (texture) {
                glActiveTexture(GL_TEXTURE0);
                texture->bind();
            }

            custom_settings();

            glBindVertexArray(vao);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            glBindVertexArray(0);
        }
        else if (Vertices == 3) {
            if (!shader)
                return;

            shader->use();

            glm::mat4 model = glm::mat4(1.0f);

            float fx = static_cast<float>(x), fy = static_cast<float>(y);
            float fw = static_cast<float>(width), fh = static_cast<float>(height);

            model = glm::translate(model, glm::vec3(fx, fy, 0.0f));

            if (rotation_angle != 0.0f) {
                model = glm::translate(model, glm::vec3(0.5f * fw, 0.5f * fh, 0.0f));
                model = glm::rotate(model, glm::radians(rotation_angle), glm::vec3(0.0f, 0.0f, 1.0f));
                model = glm::translate(model, glm::vec3(-0.5f * fw, -0.5f * fh, 0.0f));
            }

            model = glm::scale(model, glm::vec3(fw, fh, 1.0f));

            shader->set_model(model);

            custom_settings();

            glBindVertexArray(vao);
            glDrawArrays(GL_TRIANGLES, 0, 3);
            glBindVertexArray(0);
        }
    }
};

using RenderObject = RenderObjectBase<4>;

#endif // !KAKERA_ENGINE_GRAPHIC_RENDER_OBJECT
