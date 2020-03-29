#ifndef KAKERA_ENGINE_GRAPHIC_RENDER_OBJECT
#define KAKERA_ENGINE_GRAPHIC_RENDER_OBJECT

#include "shader_base.hpp"
#include "texture.h"
#include <list>
#include <utility>
#include <array>
#include <climits>
#include <variant>
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

class RenderObject
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

    GLVertices method_to_vertices(TextureApplyMethod method);
public:
    static TextureView normal_view;
    static TextureView upside_down_view;

    RenderObject();
    ~RenderObject();

    KAKERA_DISABLE_COPY(RenderObject);
    KAKERA_ENABLE_MOVE(RenderObject);

    void set_texture(Texture* texture, TextureView texture_view = normal_view);

    void set_shader(ShaderBase* shader);

    std::pair<int, int> get_size();
    void set_size(int width, int height);

    std::pair<int, int> get_position();
    void set_position(int x, int y);

    float get_rotation_angle();
    void set_rotation_angle(float angle);

    uint8_t get_opacity();
    void set_opacity(uint8_t opacity);

    void attach_color(SDL_Color color);
    void cancel_attach_color();

    Texture* get_texture();

    void render();
};

#endif // !KAKERA_ENGINE_GRAPHIC_RENDER_OBJECT
