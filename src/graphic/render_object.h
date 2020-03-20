#ifndef KAKERA_ENGINE_GRAPHIC_RENDER_OBJECT
#define KAKERA_ENGINE_GRAPHIC_RENDER_OBJECT

#include "shader_base.h"
#include "texture.h"
#include <memory>
#include <list>
#include <utility>
#include "../disable_copy_and_move.inc"

enum class TextureFillingMethod
{
    stretch,
    tile,
    repeat
};

class RenderObject
{
private:
    std::unique_ptr<Texture> texture;
    ShaderBase* shader = nullptr;

    int width = 0, height = 0;
    int x = 0, y = 0;
    float rotation_angle = 0.0f;
public:
    RenderObject* parent = nullptr;
    std::list<std::unique_ptr<RenderObject>> children;

    RenderObject();
    RenderObject(Texture&& texture, ShaderBase& shader);
    ~RenderObject();

    KAKERA_DISABLE_COPY(RenderObject);

    bool is_set_texture();
    void set_texture(Texture&& texture, TextureFillingMethod method = TextureFillingMethod::stretch);

    bool is_set_shader();
    void set_shader(ShaderBase& shader);

    std::pair<int, int> get_size();
    void set_size(int width, int height);

    std::pair<int, int> get_position();
    void set_position(int x, int y);

    float get_rotation_angle();
    void set_rotation_angle(float angle);

    void render();
};

#endif // !KAKERA_ENGINE_GRAPHIC_RENDER_OBJECT
