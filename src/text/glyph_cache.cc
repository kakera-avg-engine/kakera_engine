#include "glyph_cache.h"

GlyphCacheLine::GlyphCacheLine(Texture* texture, int y)
{
    this->texture = texture;
    this->y = y;
}

GlyphCacheLine::~GlyphCacheLine()
{
    texture = nullptr;
}

bool GlyphCacheLine::add_glyph(wchar_t ch, Glyph& glyph, unsigned char* bitmap)
{
    if (glyph.width + used_x > GlyphCache::TEXTURE_SIZE)
        return false;

    glyph.texture_position = { used_x, y, glyph.width, glyph.height };
    glyph.texture = texture;
    used_x += glyph.width + 1;
    glyphs.insert({ ch, glyph });

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    texture->update_pixels(glyph.texture_position.x, glyph.texture_position.y, glyph.width, glyph.height, bitmap);
    
    return true;
}

std::optional<Glyph> GlyphCacheLine::get_glyph(wchar_t ch)
{
    if (auto iter = glyphs.find(ch); iter != glyphs.end())
        return std::make_optional(iter->second);
    else
        return std::nullopt;
}

const int GlyphCacheLine::get_y()
{
    return y;
}

GlyphCache::GlyphCache(FT_Face face)
{
    this->face = face;

    std::string id = kakera::UUID::create();
    KAKERA_TEXTURE_MANAGER.set_texture(id, Texture(TEXTURE_SIZE, TEXTURE_SIZE, TextureFormat::red));
    texture = KAKERA_TEXTURE_MANAGER[id];
}

GlyphCache::~GlyphCache()
{
    face = nullptr;
    texture = nullptr;
}

std::optional<Glyph> GlyphCache::get_glyph(int font_size, wchar_t ch)
{
    if (glyph_lines.find(font_size) != glyph_lines.end()) {
        auto range = glyph_lines.equal_range(font_size);
        for (auto iter = range.first; iter != range.second; iter++) {
            auto& line = iter->second;
            if (auto glyph = line->get_glyph(ch); glyph.has_value()) {
                return glyph;
            }
        }

        FT_Set_Pixel_Sizes(face, 0, font_size);
        FT_Load_Char(face, ch, FT_LOAD_RENDER);

        Glyph glyph;
        glyph.width = face->glyph->bitmap.width;
        glyph.height = face->glyph->bitmap.rows;
        glyph.bearing_x = face->glyph->bitmap_left;
        glyph.bearing_y = face->glyph->bitmap.rows - face->glyph->bitmap_top;
        glyph.advance = face->glyph->advance.x >> 6;

        for (auto iter = range.first; iter != range.second; iter++) {
            auto& line = iter->second;
            if (line->add_glyph(ch, glyph, face->glyph->bitmap.buffer)) {
                return std::make_optional(glyph);
            }
        }
    }

    int used_y = 0;
    for (auto& line : glyph_lines)
        used_y += line.first;
    if ((TEXTURE_SIZE - used_y) < font_size)
        return std::nullopt;

    glyph_lines.emplace(font_size, std::make_unique<GlyphCacheLine>(texture, used_y));
    return get_glyph(font_size, ch);
}
