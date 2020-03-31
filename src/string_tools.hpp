#ifndef KAKERA_ENGINE_STRING_TOOLS
#define KAKERA_ENGINE_STRING_TOOLS

#include <string>
#include <iterator>
#include <vector>
#include <regex>

#define is_in_range(ch,min,max) (ch>=min&&ch<=max)

#ifdef _WIN32
#   define WIN32_LEAN_AND_MEAN
#   include <Windows.h>
#endif // !_WIN32

class StringTools
{
public:
    static void trim(std::string& str)
    {
        std::string::size_type pos = 0;
        for (auto iter = str.begin(); iter != str.end(); iter++) {
            if (*iter != ' ' && *iter != '\n' && *iter != '\t' && pos == 0) {
                pos = std::distance(str.begin(), iter);
                break;
            }
        }
        str.erase(0, pos);

        pos = 0;
        for (auto iter = str.rbegin(); iter != str.rend(); iter++) {
            if (*iter != ' ' && *iter != '\n' && *iter != '\t' && pos == 0) {
                pos = std::distance(str.rbegin(), iter);
                break;
            }
        }
        str.erase(str.length() - pos, pos + 1);
    }

    static void trim_multi_line(std::string& str)
    {
        std::regex pattern("<br\\s*/>");
        str = std::regex_replace(str, pattern, "\n");
        std::vector<std::string> lines;

        std::string::size_type pos = 0;
        for (int i = 0; i < str.length(); i++) {
            if (str[i] == '\n') {
                lines.push_back(str.substr(pos, i - pos));
                pos = i;
            }
        }

        for (auto& line : lines) {
            std::string::size_type s_pos = 0;
            for (auto iter = line.begin(); iter != line.end(); iter++) {
                if (*iter != ' ' && *iter != '\n' && *iter != '\t' && s_pos == 0) {
                    s_pos = std::distance(line.begin(), iter);
                    break;
                }
            }
            line.erase(0, s_pos);
        }

        str.erase();
        for (auto& line : lines) {
            str += line;
            str += "\n";
        }
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
        return is_in_range(ch, L'\u0001', L'\u024f') ||  // Latin
               is_in_range(ch, L'\u0370', L'\u052f') ||  // Greek & Cyrillic
               is_in_range(ch, L'\u1c80', L'\u1c8f') ||  // Cyrillic Extended-C
               is_in_range(ch, L'\u2de0', L'\u2dff') ||  // Cyrillic Extended-A
               is_in_range(ch, L'\ua640', L'\ua69f');    // Cyrillic Extended-B
    }
};

#endif // !KAKERA_ENGINE_STRING_TOOLS

