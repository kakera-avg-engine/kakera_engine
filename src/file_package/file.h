#ifndef KAKERA_ENGINE_FILE
#define KAKERA_ENGINE_FILE

#include "../copy_and_move.inc"

class File
{
private:
    size_t size = 0;
    char* contents = nullptr;
public:
    File();
    File(size_t size, void* contents);
    ~File();

    KAKERA_DISABLE_COPY(File);
    KAKERA_ENABLE_MOVE(File);

    bool empty();
    operator bool();
};

#endif // !KAKERA_ENGINE_FILE
