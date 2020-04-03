#ifndef KAKERA_ENGINE_COMPONENT_TEXT
#define KAKERA_ENGINE_COMPONENT_TEXT

#include "component_base.hpp"
#include "../graphic/shader_normal.hpp"
#include "glad/glad.h"
#include "../text/font_manager.h"
#include "../text/word.h"
#include "../string_tools.hpp"
#include <string>
#include <list>
#include <vector>
#include <memory>

// Text type id = 2

class Text : public Component
{
private:
    int font_size = 0;
    Font* font = nullptr;
    SDL_Color color = SDL_Color({ 0, 0, 0, 255 });
    int line_spacing = 0;

    bool is_scissor = false;

    struct Line
    {
        int height = 0;
        int origin = 0;
        std::list<std::unique_ptr<Word>> words;
    };

    std::list<Line> lines;

    void real_render()
    {
        int word_x = x;
        int word_y = y;
        for (auto& line : lines) {
            word_y += (line.height - font_size);

            for (auto& word : line.words) {
                word->render(word_x, word_y, line.origin, color);
                word_x += word->get_width();
            }

            word_x = x;
            word_y += font_size;
            word_y += line_spacing;
        }
    }
public:
    Text() = default;
    
    Text(Text&& other) noexcept
    {
        width = other.width;
        height = other.height;
        x = other.x;
        y = other.y;
        uuid = other.uuid;
        mouse_entered = other.mouse_entered;
        font_size = other.font_size;
        font = other.font;
        color = other.color;
        line_spacing = other.line_spacing;
        is_scissor = other.is_scissor;
        lines = std::move(other.lines);

        other.width = 0;
        other.height = 0;
        other.x = 0;
        other.y = 0;
        other.uuid = "";
        other.mouse_entered = false;
        other.font_size = 0;
        other.font = nullptr;
        other.color = SDL_Color({ 0, 0, 0, 255 });
        other.line_spacing = 0;
        other.is_scissor = false;
    }

    ~Text()
    {
        font = nullptr;
    }

    Text& operator=(Text&& other) noexcept
    {
        if (this != &other) {
            width = other.width;
            height = other.height;
            x = other.x;
            y = other.y;
            uuid = other.uuid;
            mouse_entered = other.mouse_entered;
            font_size = other.font_size;
            font = other.font;
            color = other.color;
            line_spacing = other.line_spacing;
            is_scissor = other.is_scissor;
            lines = std::move(other.lines);

            other.width = 0;
            other.height = 0;
            other.x = 0;
            other.y = 0;
            other.uuid = "";
            other.mouse_entered = false;
            other.font_size = 0;
            other.font = nullptr;
            other.color = SDL_Color({ 0, 0, 0, 255 });
            other.line_spacing = 0;
            other.is_scissor = false;
        }

        return *this;
    }

    void set_string(std::wstring unicode_str)
    {
        std::wstring temp;
        auto str_ptr = unicode_str.c_str();
        bool ruby_exp = false;
        std::vector<std::unique_ptr<Word>> words;

        for (auto& ch : unicode_str) {

            // Ruby expression
            if (temp == L"[[") {
                ruby_exp = true;
            }

            if (ruby_exp) {
                if (temp.back() != L'\u0029')
                    temp += ch;
                else {
                    std::wstring::size_type str_last_pos = temp.find_first_of(L'\u005D');
                    std::wstring word = temp.substr(2, str_last_pos - 2);
                    std::wstring::size_type ruby_start_pos = temp.find(L'\u0028') + 1;
                    std::wstring ruby = temp.substr(ruby_start_pos, temp.length() - ruby_start_pos - 1);
                    words.emplace_back(std::make_unique<Word>(font, font_size, word, ruby));
                    temp.clear();
                    ruby_exp = false;
                }
            }

            // Not ruby expression
            if (!ruby_exp) {
                if (StringTools::is_alphabet(ch) && ch != L'\u0020' && ch != L'\u000a') {
                    temp += ch;
                }
                else if (ch == L'\u0020') {
                    if (temp.empty()) {
                        temp = ch;
                    }
                    words.emplace_back(std::make_unique<Word>(font, font_size, temp));
                    words.emplace_back(std::make_unique<Word>(font, font_size, std::wstring(L"\u0020")));
                    temp.clear();
                }
                else if (ch == L'\u000a' || ch == L'\u0000') {
                    if (!temp.empty()) {
                        words.emplace_back(std::make_unique<Word>(font, font_size, temp));
                        temp.clear();
                    }
                    words.emplace_back(std::make_unique<Word>());
                }
                else {
                    if (!temp.empty()) {
                        words.emplace_back(std::make_unique<Word>(font, font_size, temp));
                        temp.clear();
                    }
                    temp = ch;
                    words.emplace_back(std::make_unique<Word>(font, font_size, temp));
                    temp.clear();
                }
            }
        }

        // Set lines
        bool height_resized = false;
        int temp_width = 0, temp_height = font_size;
        int i = 0, pos = 0;

        while (i < words.size()) {
            auto& word = words[i];
            int word_width = word->get_width();

            if ((width != 0 && (temp_width + word_width > width)) /* Over width */ ||
                word_width == 0 /* line breaker*/) {

                auto& line = lines.emplace_back(Line());
                line.height = temp_height;

                for (int j = pos; j < i; j++) {
                    int wby = line.words.emplace_back(std::move(words[j]))->get_bearing_y();
                    if (wby > line.origin)
                        line.origin = wby;
                }

                if (height_resized) {
                    height_resized = false;
                    temp_height = font_size;
                }

                pos = i;
                temp_width = 0;

                if(word_width == 0 || words[i + 1]->get_width() == 0)
                    i++;
            }
            else {
                if (word->has_ruby() && !height_resized) {
                    temp_height += temp_height / Word::RUBY_SIZE_BASE;
                    temp_height += temp_height / Word::RUBY_LINE_SPACING_BASE;
                    height_resized = true;
                }

                temp_width += word_width;

                i++;
            }
        }
        
    }

    void set_string(std::string str)
    {
        std::wstring unicode_str = StringTools::utf8_to_unicode(str);
        set_string(unicode_str);
    }

    void set_color(std::string color)
    {
        this->color.r = std::stoi(color.substr(1, 2), nullptr, 16);
        this->color.g = std::stoi(color.substr(3, 2), nullptr, 16);
        this->color.b = std::stoi(color.substr(5, 2), nullptr, 16);
    }

    void set_fontsize(int font_size)
    {
        this->font_size = font_size;
        line_spacing = font_size / Word::LINE_SPACING_BASE;
    }

    void set_font(std::string font_id)
    {
        font = KAKERA_FONT_MANAGER[font_id];
    }

    void set_line_spacing(int spacing)
    {
        line_spacing += spacing;
    }

    void set_size(int w, int h) override
    {
        Component::set_size(w, h);

        if (w > 0 && h > 0)
            is_scissor = true;
    }

    void render() override
    {
        if (is_scissor) {
            int gl_y = KAKERA_SHADER_NORMAL.get_window_size().second - y - height;
            glEnable(GL_SCISSOR_TEST);
            glScissor(x, gl_y, width, height);

            real_render();

            glDisable(GL_SCISSOR_TEST);
        }
        else {
            if (width == 0) {
                for (auto& line : lines) {
                    int line_width = 0;
                    for (auto& word : line.words) {
                        line_width += word->get_width();
                    }

                    if (line_width > width)
                        width = line_width;
                }
            }

            if (height == 0) {
                for (auto& line : lines) {
                    height += line.height;
                }

                height += ((lines.size() - 1) * line_spacing);
            }

            real_render();
        }
    }
};

#endif // !KAKERA_ENGINE_COMPONENT_TEXT
