#ifndef KAKERA_ENGINE_TEXT_GLYPH
#define KAKERA_ENGINE_TEXT_GLYPH

#include "SDL.h"
#include "../graphic/texture.h"

struct Glyph
{
    int width, height;
    int bearing_x, bearing_y;
    int advance;
    SDL_Rect texture_position;
    Texture* texture = nullptr;
};

#endif // !KAKERA_ENGINE_TEXT_GLYPH
