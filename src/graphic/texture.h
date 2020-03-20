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
    unsigned int tex_id = UINT_MAX;
    GLenum gl_format = GL_RGB;
public:
    Texture(int width, int height, TextureFormat format = TextureFormat::rgb);
    ~Texture();

    KAKERA_DISABLE_COPY(Texture);
    KAKERA_ENABLE_MOVE(Texture);

    void update_pixels(unsigned char* pixels);
    void update_pixels(int x, int y, int width, int height, unsigned char* pixels);

    const unsigned int get_id();
};

#endif // !KAKERA_ENGINE_GRAPHIC_TEXTURE
