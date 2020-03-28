#ifndef KAKERA_ENGINE_PACKAGE
#define KAKERA_ENGINE_PACKAGE

#ifdef _WIN32
#   define WIN32_LEAN_AND_MEAN
#   include <Windows.h>
#else
#endif // _WIN32

#if (!defined(_DEBUG) || defined(NDEBUG))
#   include "file_index.h"
#   include "msgpack.hpp"
#endif

#include "file.h"
#include "../log/log.h"
#include <unordered_map>
#include <string>
#include <optional>
#include <filesystem>
#include "../copy_and_move.inc"

class Package
{
private:
#if (!defined(_DEBUG) || defined(NDEBUG))
    std::unordered_map<std::string, FileIndex> file_index;
#   ifdef _WIN32
    // Windows Release
    HANDLE h_file = nullptr;
    HANDLE h_map = nullptr;
#   else
    // Linux Release
#   endif
#else
    // Debug
    std::filesystem::path root_path;
#endif
    
    void create_file_index();
public:
    Package() = default;
    Package(std::string src);
    ~Package();

    KAKERA_DISABLE_COPY(Package);
    KAKERA_ENABLE_MOVE(Package);

    void open(std::string src);
    void close();

    std::optional<File> get_file(std::string filename);
    std::optional<File> operator[](std::string filename);
};

#endif // !KAKERA_ENGINE_PACKAGE
