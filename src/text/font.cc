#include "font.h"

Font::Font(FT_Library ft_lib, std::string src)
{
    this->ft_lib = ft_lib;

    auto font = KAKERA_PACKAGE_MANAGER.get_file(src);
    if (font.has_value()) {
        font_file = std::move(font.value());
        FT_Error error = FT_New_Memory_Face(this->ft_lib, (const FT_Byte*)*font_file, font_file.size(), 0, &face);
        if (error) {
            KAKERA_LOG(LogLevel::Error, "Cannot load font file \"{}\".", src);
        }
    }
}

Font::~Font()
{
    FT_Done_Face(face);
    face = nullptr;
    ft_lib = nullptr;
}

Glyph Font::get_glyph(int font_size, wchar_t ch)
{
    if (!caches.empty()) {
        for (auto& cache : caches) {
            if (auto glyph = cache->get_glyph(font_size, ch); glyph.has_value())
                return glyph.value();
        }
    }

    caches.push_back(std::make_unique<GlyphCache>(face));
    return get_glyph(font_size, ch);
}
