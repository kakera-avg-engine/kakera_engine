#ifndef KAKERA_ENGINE_FILE_INDEX
#define KAKERA_ENGINE_FILE_INDEX

#include <string>

struct FileIndex
{
    std::string filename;
    std::string uuid;
    size_t size;
    size_t offset;
};

#endif // !KAKERA_ENGINE_FILE_INDEX
