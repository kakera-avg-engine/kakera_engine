#ifndef KAKERA_ENGINE_STRING_TRIM
#define KAKERA_ENGINE_STRING_TRIM

#include <string>
#include <iterator>

class StringTrim
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
};

#endif // !KAKERA_ENGINE_STRING_TRIM

