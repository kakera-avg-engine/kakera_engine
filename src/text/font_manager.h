#ifndef KAKERA_ENGINE_TEXT_FONT_MANAGER
#define KAKERA_ENGINE_TEXT_FONT_MANAGER

#include <string>
#include <unordered_map>
#include <memory>
#include "font.h"
#include "../copy_and_move.inc"

#define KAKERA_FONT_MANAGER FontManager::get()

class FontManager
{
private:
    std::unordered_map<std::string, std::unique_ptr<Font>> fonts;

    FontManager();
    ~FontManager();
public:
    KAKERA_DISABLE_COPY(FontManager);
    KAKERA_DISABLE_MOVE(FontManager);

    void set_font();
    Font* get_font(std::string id);
    Font* operator[](std::string id);

    static FontManager& get();
};

#endif // !KAKERA_ENGINE_TEXT_FONT_MANAGER
