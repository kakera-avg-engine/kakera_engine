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
    }
}

Font::~Font()
{
}
