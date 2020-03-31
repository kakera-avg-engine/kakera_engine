#ifndef KAKERA_ENGINE_TEXT_FONT
#define KAKERA_ENGINE_TEXT_FONT

#include <string>
#include <list>
#include <unordered_map>
#include <memory>
#include "ft2build.h"
#include FT_FREETYPE_H
#include "glyph_cache.h"
#include "../log/log.h"
#include "../file_package/package_manager.h"
#include "../copy_and_move.inc"

class Font
{
private:
    FT_Library ft_lib = nullptr;
    FT_Face face = nullptr;

    std::list<std::unique_ptr<GlyphCache>> caches;
public:
    Font(FT_Library ft_lib, std::string src);
    ~Font();

    KAKERA_DISABLE_COPY(Font);
    KAKERA_DISABLE_MOVE(Font);

    Glyph get_glyph(int font_size, wchar_t ch);
};

#endif // !KAKERA_ENGINE_TEXT_FONT