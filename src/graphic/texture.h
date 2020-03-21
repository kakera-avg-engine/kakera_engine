#ifndef KAKERA_ENGINE_GRAPHIC_TEXTURE
#define KAKERA_ENGINE_GRAPHIC_TEXTURE

#include "glad/glad.h"
#include <climits>
#include "../copy_and_move.inc"

enum class TextureFormat
{
    red,
    rgb,
    rgba
};

class Texture
{
private:
    int w = 0, h = 0;
    GLuint tex_id = UINT_MAX;
    GLenum gl_format = GL_RGB;
public:
    Texture(int width, int height, TextureFormat format = TextureFormat::rgb);
    ~Texture();

    KAKERA_DISABLE_COPY(Texture);
    KAKERA_ENABLE_MOVE(Texture);

    void update_pixels(unsigned char* pixels);
    void update_pixels(int x, int y, int width, int height, unsigned char* pixels);

    const GLuint get_id();

    void bind();
};

#endif // !KAKERA_ENGINE_GRAPHIC_TEXTURE
