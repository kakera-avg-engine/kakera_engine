#ifndef KAKERA_ENGINE_COMPONENT_SINGLE_LINE_TEXT
#define KAKERA_ENGINE_COMPONENT_SINGLE_LINE_TEXT

#include "component_base.hpp"
#include "../text/font_manager.h"
#include "../text/word.h"
#include "../string_tools.hpp"
#include <string>
#include <memory>
#include <list>

#define is_in_range(ch,min,max) (ch>=min&&ch<=max)

// SingleLineText type id = 3

class SingleLineText : public Component
{
private:
    Font* font = nullptr;
    std::list<std::unique_ptr<Word>> words;
    SDL_Color color = { 0, 0, 0, 255 };

    int get_origin() {
        int result = 0;

        for (auto& word : words) {
            if (int wby = word->get_bearing_y(); wby > result)
                result = wby;
        }

        return result;
    }
public:
    SingleLineText() = default;

    SingleLineText(int font_size, std::string str)
    {
        set_fontsize(font_size);
        set_string(str);
    }

    SingleLineText(SingleLineText&& other) noexcept
    {
        font = other.font;
        color = other.color;
        words = std::move(other.words);

        other.font = nullptr;
        other.color = { 0, 0, 0, 255 };
    }

    ~SingleLineText()
    {
        font = nullptr;
    }

    SingleLineText& operator=(SingleLineText&& other) noexcept
    {
        if (this != &other) {
            font = other.font;
            color = other.color;
            words = std::move(other.words);

            other.font = nullptr;
            other.color = { 0, 0, 0, 255 };
        }

        return *this;
    }

    void set_string(std::wstring unicode_str)
    {
        std::wstring temp;
        auto str_ptr = unicode_str.c_str();
        bool ruby_exp = false;
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
                    words.emplace_back(std::make_unique<Word>(font, height, word, ruby));
                    temp.clear();
                    ruby_exp = false;
                }
            }

            // Not ruby expression
            if (!ruby_exp) {
                if (StringTools::is_alphabet(ch) && ch != L'\u0020') {
                    temp += ch;
                }
                else if (ch == L'\u0020' || ch == L'\u0000') {
                    if (temp.empty()) {
                        temp = ch;
                    }
                    words.emplace_back(std::make_unique<Word>(font, height, temp));
                    if (ch == L'\u0020')
                        words.emplace_back(std::make_unique<Word>(font, height, std::wstring(L"\u0020")));
                    temp.clear();
                }
                else {
                    temp = ch;
                    words.emplace_back(std::make_unique<Word>(font, height, temp));
                    temp.clear();
                }
            }
        }

        // Calculate size
        bool height_resized = false;
        for (auto& word : words) {
            if (word->has_ruby() && !height_resized) {
                this->height += this->height / Word::RUBY_SIZE_BASE;
                this->height += this->height / Word::RUBY_LINE_SPACING_BASE;
                height_resized = true;
            }

            this->width += word->get_width();
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
        this->height = font_size;
    }

    void set_font(std::string font_id)
    {
        font = KAKERA_FONT_MANAGER[font_id];
    }

    void set_font(Font* font) {
        this->font = font;
    }

    void set_spacing(int spacing)
    {
        for (auto& word : words) {
            word->set_spacing(spacing);
        }
    }

    void render() override
    {
        int origin = get_origin();
        int word_x = x;
        for (auto& word : words) {
            word->render(word_x, y, origin, color);
            word_x += word->get_width();
        }
    }
};

#endif // !KAKERA_ENGINE_COMPONENT_SINGLE_LINE_TEXT
