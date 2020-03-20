#ifndef KAKERA_ENGINE_GRAPHIC_TEXTURE
#define KAKERA_ENGINE_GRAPHIC_TEXTURE

#include "glad/glad.h"
#include "../disable_copy_and_move.inc"

class Texture
{
private:
    unsigned int texid;
public:
    Texture(int width, int height, bool enable_alpha = false, bool upside_down = false);
    ~Texture();

    void update_pixels(unsigned char* pixels);
    void update_pixels(int x, int y, int width, int height, unsigned char* pixels);

    unsigned int get_id();
};

#endif // !KAKERA_ENGINE_GRAPHIC_TEXTURE
