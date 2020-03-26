#ifndef KAKERA_ENGINE_LOG
#define KAKERA_ENGINE_LOG

#include "fmt/format.h"
#include <mutex>
#include <fstream>
#include <filesystem>
#include <ctime>
#include <string>
#include <unordered_map>
#include <cstdlib>
#ifdef _WIN32
#   include <process.h>
#endif
#include "../copy_and_move.inc"
#include "../version.inc"

#define KAKERA_LOG(level,what) Log::get().print(level,what,__FILE__,__LINE__)

enum class LogLevel
{
    Info,
    Debug,
    Warning,
    Error,
    Fatal
};

class Log
{
private:
    static std::mutex m_lock;
    static std::unordered_map<LogLevel, std::string> level_map;
    std::ofstream log_file;

    Log();
    ~Log();

    static std::string now_time();
public:
    void print(LogLevel level, std::string what, std::string file, int line);

    static Log& get();
};

#endif // !KAKERA_ENGINE_LOG
