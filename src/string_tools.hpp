#ifndef KAKERA_ENGINE_STRING_TOOLS
#define KAKERA_ENGINE_STRING_TOOLS

#include <string>
#include <iterator>
#include <vector>

#define is_in_range(ch,min,max) (ch>=min&&ch<=max)

#ifdef _WIN32
#   define WIN32_LEAN_AND_MEAN
#   include <Windows.h>
#endif // !_WIN32

class StringTools
{
private:
    static void trim_single_line(std::string& str)
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

    static bool is_all_space(std::string& str)
    {
        bool result = true;
        for (auto& ch : str) {
            if (ch != ' ' && ch != '\n' && ch != '\t') {
                result = false;
                break;
            }
        }

        return result;
    }
public:
    static void trim(std::string& str)
    {
        trim_single_line(str);
        str += '\0';

        if (str.find('\n') != std::string::npos) {

            std::vector<std::string> lines;

            std::string::size_type pos = 0;
            for (std::string::size_type i = 0; i < str.length(); i++) {
                if (str[i] == '\n' || str[i] == '\0') {
                    lines.push_back(str.substr(pos, i - pos + 1));
                    pos = i + 1;
                }
            }
            str.erase();

            for (auto& line : lines) {
                if (!is_all_space(line)) {
                    trim_single_line(line);

                    if (line.back() != '\0')
                        line += '\n';
                }
                else
                    line = "\n";
                
                str += line;
            }
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

