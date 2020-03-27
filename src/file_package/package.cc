#include "package.h"

void Package::create_file_index()
{
#if (!defined(_DEBUG) || defined(NDEBUG))
#   ifdef _WIN32
    // Windows Release
#   else
    // Linux Release
#   endif
#endif
}

Package::Package(const char* src)
{
#if (!defined(_DEBUG) || defined(NDEBUG))
#   ifdef _WIN32
    // Windows Release
#   else
    // Linux Release
#   endif
#else
    // Debug
    std::filesystem::path path(src);
    if (!std::filesystem::exists(path)) KAKERA_LOG(LogLevel::Fatal, "No such directory found.");
    if (!std::filesystem::is_directory(path)) KAKERA_LOG(LogLevel::Fatal, "Given path:\"{}\" is not a directory.", src);
    root_path = path;
#endif
}

Package::Package(Package&& other)
{
#if (!defined(_DEBUG) || defined(NDEBUG))
#   ifdef _WIN32
    // Windows Release
#   else
    // Linux Release
#   endif
#else
    // Debug
    root_path == std::move(other.root_path);
#endif
}

Package::~Package()
{
#if (!defined(_DEBUG) || defined(NDEBUG))
#   ifdef _WIN32
    // Windows Release
#   else
    // Linux Release
#   endif
#endif
}

Package& Package::operator=(Package&& other)
{
    if (this != &other) {
#if (!defined(_DEBUG) || defined(NDEBUG))
#   ifdef _WIN32
        // Windows Release
#   else
        // Linux Release
#   endif
#else
        // Debug
        root_path == std::move(other.root_path);
#endif
    }

    return *this;
}

void Package::open(const char* src)
{
#if (!defined(_DEBUG) || defined(NDEBUG))
#   ifdef _WIN32
    // Windows Release
#   else
    // Linux Release
#   endif
#else
    // Debug
    if (root_path.empty()) {
        std::filesystem::path path(src);
        if (!std::filesystem::exists(path)) KAKERA_LOG(LogLevel::Fatal, "No such directory found.");
        if (!std::filesystem::is_directory(path)) KAKERA_LOG(LogLevel::Fatal, "Given path:\"{}\" is not a directory.", src);
        root_path = path;
    }
#endif
}

void Package::close()
{
#if (!defined(_DEBUG) || defined(NDEBUG))
#   ifdef _WIN32
    // Windows Release
#   else
    // Linux Release
#   endif
#else
    // Debug
    root_path.clear();
#endif
}

std::optional<File> Package::get_file(const char* filename)
{
#if (!defined(_DEBUG) || defined(NDEBUG))
#   ifdef _WIN32
    // Windows Release
#   else
    // Linux Release
#   endif
#else
    // Debug
    std::filesystem::path path(filename);
    path = root_path / path;

    if (!std::filesystem::exists(path)) {
        KAKERA_LOG(LogLevel::Error, "No such file \"{}\" found in given package.");
        return std::nullopt;
    }

    size_t size;
    char* buffer;

    std::ifstream file(path, std::ios::binary);

    file.seekg(0, std::ios_base::end);
    size = file.tellg();
    file.seekg(0);

    buffer = new char[size];
    file.read(buffer, size);

    file.close();

    return std::make_optional(File(size, buffer));
#endif
}

std::optional<File> Package::operator[](const char* filename)
{
    return get_file(filename);
}
