#include "font.h"

Font::Font(FT_Library ft_lib, std::string src)
{
    this->ft_lib = ft_lib;

    auto font_file = KAKERA_PACKAGE_MANAGER.get_file(src);
    if (font_file.has_value()) {
        File file = std::move(font_file.value());
        FT_Error error = FT_New_Memory_Face(this->ft_lib, (const FT_Byte*)*file, file.size(), 0, &face);
        if (error) {
            KAKERA_LOG(LogLevel::Error, "Cannot load font file \"{}\".", src);
        }
        KAKERA_PACKAGE_MANAGER.add_persistent_file(std::move(file));
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
