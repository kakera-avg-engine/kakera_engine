#ifndef KAKERA_ENGINE_TEXT_GLYPH_CACHE
#define KAKERA_ENGINE_TEXT_GLYPH_CACHE

#include "glyph.h"
#include <unordered_map>
#include <optional>
#include <string>
#include <memory>
#include "ft2build.h"
#include FT_FREETYPE_H
#include "glad/glad.h"
#include "../graphic/texture_manager.h"
#include "../copy_and_move.inc"
#include "../uuid.hpp"

class GlyphCacheLine
{
private:
    Texture* texture = nullptr;
    int used_x = 0, y = 0;
    std::unordered_map<wchar_t, Glyph> glyphs;
public:
    GlyphCacheLine(Texture* texture, int y);
    ~GlyphCacheLine();

    bool add_glyph(wchar_t ch, Glyph& glyph, unsigned char* bitmap);
    std::optional<Glyph> get_glyph(wchar_t ch);

    const int get_y();
};

class GlyphCache
{
private:
    Texture* texture = nullptr;
    FT_Face face = nullptr;
    std::unordered_multimap<int, std::unique_ptr<GlyphCacheLine>> glyph_lines;
public:
    static constexpr int TEXTURE_SIZE = 2048;

    GlyphCache(FT_Face face);
    ~GlyphCache();

    std::optional<Glyph> get_glyph(int font_size, wchar_t ch);
};

#endif // !KAKERA_ENGINE_TEXT_GLYPH_CACHE
