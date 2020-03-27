#ifndef KAKERA_ENGINE_FILE
#define KAKERA_ENGINE_FILE

#include <memory>
#include "../copy_and_move.inc"

class File
{
private:
    size_t _size = 0;
    std::unique_ptr<char> file_ptr;
public:
    File() = default;
    File(size_t size, char* file_ptr);
    ~File() = default;

    KAKERA_DISABLE_COPY(File);
    KAKERA_ENABLE_MOVE(File);

    bool empty();
    operator bool();

    size_t size();

    char* get();
    char* operator*();
};

#endif // !KAKERA_ENGINE_FILE
