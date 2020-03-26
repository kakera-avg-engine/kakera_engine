#ifndef KAKERA_ENGINE_PACKAGE
#define KAKERA_ENGINE_PACKAGE

#include <memory>
#include "file.h"
#include "../copy_and_move.inc"

class Package
{
private:
public:
    Package();
    Package(const char* src);
    ~Package();

    KAKERA_DISABLE_COPY(Package);
    KAKERA_ENABLE_MOVE(Package);

    void open(const char* src);
    void close();

    bool empty();
    operator bool();

    std::unique_ptr<File> get_file(const char* filename);
    std::unique_ptr<File> operator[](const char* filename);
};

#endif // !KAKERA_ENGINE_PACKAGE
