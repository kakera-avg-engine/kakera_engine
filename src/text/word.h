#ifndef KAKERA_ENGINE_TEXT_WORD
#define KAKERA_ENGINE_TEXT_WORD

#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include "font.h"
#include "../graphic/render_object.h"
#include "../graphic/shader_text.hpp"
#include "SDL.h"

#ifdef _WIN32
#   undef max
#endif

class Word
{
private:
    int font_size = 0;
    int spacing = 0;
    int ruby_offset = 0;
    int offset = 0;

    std::vector<Glyph> word;
    std::vector<std::unique_ptr<RenderObject>> word_render_objs;

    std::unique_ptr<Word> ruby;
public:
    Word(Font* font, int font_size, std::wstring word);
    Word(Font* font, int font_size, std::wstring word, std::wstring ruby);
    ~Word() = default;

    bool has_ruby();

    void set_spacing(int spacing);

    int get_width();
    int get_bearing_y();

    int length();

    void render(int x, int y, int origin, SDL_Color color); // return last advance

    static constexpr int RUBY_SIZE_BASE = 2;
    static constexpr int RUBY_LINE_SPACING_BASE = 8;
};

#endif // !KAKERA_ENGINE_TEXT_WORD
