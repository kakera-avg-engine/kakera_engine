#ifndef KAKERA_ENGINE_TEXT_FONT
#define KAKERA_ENGINE_TEXT_FONT

#include <string>
#include <list>
#include "../copy_and_move.inc"

class Font
{
private:
public:
    Font();
    Font(const char* src);
    ~Font();

    KAKERA_DISABLE_COPY(Font);
    KAKERA_ENABLE_MOVE(Font);

    void open(const char* src);
};

#endif // !KAKERA_ENGINE_TEXT_FONT