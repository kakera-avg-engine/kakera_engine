#include "font_manager.h"

FontManager::FontManager()
{
    FT_Init_FreeType(&ft_lib);
}

FontManager::~FontManager()
{
    for (auto& font : fonts) {
        Font* font_raw_ptr = font.second.release();
        delete font_raw_ptr;
        font_raw_ptr = nullptr;
    }
    FT_Done_FreeType(ft_lib);
}

void FontManager::set_font(std::string id, std::string src)
{
    fonts.try_emplace(id, std::make_unique<Font>(ft_lib, src));
}

Font* FontManager::get_font(std::string id)
{
    if (auto& iter = fonts.find(id); iter != fonts.end())
        return iter->second.get();
    return nullptr;
}

Font* FontManager::operator[](std::string id)
{
    return get_font(id);
}

FontManager& FontManager::get()
{
    static FontManager instance;
    return instance;
}
