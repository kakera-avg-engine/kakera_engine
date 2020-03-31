#ifndef KAKERA_ENGINE_COMPONENT_MULTI_LINE_TEXT
#define KAKERA_ENGINE_COMPONENT_MULTI_LINE_TEXT

#include "component_base.hpp"
#include "../text/font_manager.h"
#include "../text/word.h"
#include "../string_tools.hpp"
#include <string>
#include <list>
#include <vector>
#include <memory>

// MultiLineText type id = 4

class MultiLineText : public Component
{
private:
    int font_size = 0;
    Font* font = nullptr;
    SDL_Color color = { 0, 0, 0, 255 };
    int line_spacing = 0;

    struct Line
    {
        int height = 0;
        int origin = 0;
        std::list<std::unique_ptr<Word>> words;
    };

    std::list<Line> lines;
public:
    MultiLineText() = default;

    ~MultiLineText()
    {
        font = nullptr;
    }

    void set_string(std::wstring unicode_str)
    {
        std::wstring temp;
        auto str_ptr = unicode_str.c_str();
        bool ruby_exp = false;
        std::vector<std::unique_ptr<Word>> words;

        for (int i = 0; i <= unicode_str.size(); i++) {
            wchar_t ch = str_ptr[i];

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
                else if (ch == L'\u0000') {
                    if (!temp.empty()) {
                        words.emplace_back(std::make_unique<Word>(font, font_size, temp));
                        temp.clear();
                    }
                    words.emplace_back(std::make_unique<Word>());
                }
                else if (ch == L'\u000a') {
                    if (!temp.empty()) {
                        words.emplace_back(std::make_unique<Word>(font, font_size, temp));
                        temp.clear();
                    }
                    words.emplace_back(std::make_unique<Word>());
                }
                else {
                    temp = ch;
                    words.emplace_back(std::make_unique<Word>(font, font_size, temp));
                    temp.clear();
                }
            }
        }

        // Set lines
        bool height_resized = false;
        int temp_width = 0, temp_height = font_size;
        int pos = 0;
        for (int i = 0; i < words.size(); i++) {
            auto& word = words[i];

            if ((width != 0 && temp_width + word->get_width() > width) /* Over width */||
                word->get_width() == 0 /* line breaker */) {

                lines.emplace_back(Line());
                auto& line = lines.back();
                line.height = temp_height;

                for (int j = pos; j < i; j++) {
                    auto& line_word = words[j];
                    line.words.emplace_back(std::move(line_word));

                    if (int wby = line_word->get_bearing_y(); wby > line.origin)
                        line.origin = wby;
                }

                if (height_resized) {
                    height_resized = false;
                    temp_height = font_size;
                }

                pos = i;
            }
            else {
                if (word->has_ruby() && !height_resized) {
                    temp_height += temp_height / Word::RUBY_SIZE_BASE;
                    temp_height += temp_height / Word::RUBY_LINE_SPACING_BASE;
                    height_resized = true;
                }

                temp_width += word->get_width();
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

    void set_spacing(int spacing)
    {
        line_spacing += spacing;
    }

    void render() override
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
};

#endif // !KAKERA_ENGINE_COMPONENT_MULTI_LINE_TEXT
