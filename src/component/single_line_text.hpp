#ifndef KAKERA_ENGINE_COMPONENT_SINGLE_LINE_TEXT
#define KAKERA_ENGINE_COMPONENT_SINGLE_LINE_TEXT

#ifdef _WIN32
#   define WIN32_LEAN_AND_MEAN
#   include <Windows.h>
#endif // !_WIN32

#include "fmt/format.h"

#include "component_base.hpp"
#include "../text/font_manager.h"
#include "../text/word.h"
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

    static std::wstring utf8_to_unicode(std::string utf8_str)
    {
        std::wstring result;
#ifdef _WIN32
        int size = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, utf8_str.data(), utf8_str.length(), nullptr, 0);

        result.resize(size);

        MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, utf8_str.data(), utf8_str.length(), &result[0], size);
#endif
        return result;
    }

    static bool is_alphabet(wchar_t ch)
    {
        return is_in_range(ch, L'\u0001', L'\u024f') || // Latin
               is_in_range(ch, L'\u0370', L'\u052f') ;  // Greek & Cyrillic
    }
public:
    SingleLineText() = default;

    SingleLineText(int font_size, std::string str)
    {
        set_fontsize(font_size);
        set_string(str);
    }

    SingleLineText(SingleLineText&& other)
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

    SingleLineText& operator=(SingleLineText&& other)
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

    void set_string(std::string str)
    {
        std::wstring unicode_str = utf8_to_unicode(str);
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
                if (is_alphabet(ch) && ch != L'\u0020') {
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

        for (auto& word : words) {
            if (word->has_ruby()) {
                this->height += this->height / Word::RUBY_SIZE_BASE;
                this->height += this->height / Word::RUBY_LINE_SPACING_BASE;
                break;
            }
        }
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

    void set_spacing(int spacing)
    {
        for (auto& word : words) {
            word->set_spacing(spacing);
        }
    }

    int get_width()
    {
        int result = 0;
        for (auto& word : words)
            result += word->get_width();
        
        return result;
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
