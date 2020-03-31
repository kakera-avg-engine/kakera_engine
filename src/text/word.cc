#include "word.h"

Word::Word(Font* font, int font_size, std::wstring word)
{
    this->font_size = font_size;

    for (auto ch : word) {
        Glyph glyph = font->get_glyph(font_size, ch);
        this->word.push_back(glyph);

        word_render_objs.emplace_back(std::make_unique<RenderObject>());
        RenderObject* render_obj = word_render_objs.back().get();
        render_obj->set_size(glyph.width, glyph.height);
        render_obj->set_texture(glyph.texture, glyph.texture->get_area_coord(glyph.texture_position));
        render_obj->set_shader(&KAKERA_SHADER_TEXT);
    }
}

Word::Word(Font* font, int font_size, std::wstring word, std::wstring ruby) : Word(font, font_size, word)
{
    this->ruby = std::make_unique<Word>(font, font_size / RUBY_SIZE_BASE, ruby);

    int ww = get_width();
    int wr = this->ruby->get_width();

    if (ww > wr)
    {
        if (word.size() > 1)
            this->ruby->set_spacing((ww - wr) / this->ruby->length());
        else
            ruby_offset = (ww - wr) / 2;
    }
    else
    {
        if (word.size() > 1)
            set_spacing((wr - ww) / length());
        else
            offset = (wr - ww) / 2;
    }
}

bool Word::has_ruby()
{
    return ruby != nullptr;
}

void Word::set_spacing(int spacing)
{
    this->spacing += spacing;

    if (ruby)
        ruby->set_spacing(spacing / RUBY_SIZE_BASE);
}

int Word::get_width()
{
    if (word.empty())
        return 0;

    int result = 0;
    for (auto glyph : word)
        result += (glyph.advance + spacing);
    result += offset * 2;

    return result;
}

int Word::get_bearing_y()
{
    if (word.empty())
        return 0;

    int result = 0;
    for (auto glyph : word) {
        if (glyph.bearing_y > result);
        result = glyph.bearing_y;
    }

    return result;
}

int Word::length()
{
    return word.size();
}

void Word::render(int x, int y, int origin, SDL_Color color)
{
    int raw_y = y;

    if (has_ruby()) {
        int ruby_font_size = font_size / RUBY_SIZE_BASE;
        int ruby_line_spacing = font_size / RUBY_LINE_SPACING_BASE;
        ruby->render(x + ruby_offset, y - ruby_font_size - ruby_line_spacing, ruby->get_bearing_y(), color);
    }
    
    x += offset;
    for (int i = 0; i < word.size(); i++) {
        Glyph glyph = word[i];
        RenderObject* render_obj = word_render_objs[i].get();

        x += glyph.bearing_x;
        y += (font_size - glyph.height - origin) + glyph.bearing_y;

        render_obj->set_position(x, y);
        render_obj->attach_color(color);

        render_obj->render();

        x += (glyph.advance + spacing);
        y = raw_y;
    }
}
