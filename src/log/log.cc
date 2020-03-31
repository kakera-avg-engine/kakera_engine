#include "log.h"

std::mutex Log::m_lock;
std::unordered_map<LogLevel, std::string> Log::level_map({
    { LogLevel::Info, "INFO" },
    { LogLevel::Debug, "DEBUG" },
    { LogLevel::Warning, "WARNING" },
    { LogLevel::Error, "ERROR" },
    { LogLevel::Fatal, "FATAL" }
});

Log::Log()
{
    if (!std::filesystem::exists("log/")) std::filesystem::create_directory("log/");
    std::time_t now = std::time(0);
    std::string time_string = now_time();
    int pid_id = (int)getpid();
    std::string status;
#if (!defined(_DEBUG) || defined(NDEBUG))
    status = "release";
#else
    status = "debug";
#endif
    std::string file_name = fmt::format("kakera_{0}_log_{1}_{2}.log", status, now, pid_id);
    file_name = "log/" + file_name;
    log_file.open(file_name);

    log_file << fmt::format("Kakera AVG Engine Version {0}.{1} {2}\n", VERSION_MAJOR, VERSION_MINOR, VERSION_NAME);
    log_file << fmt::format("Log create on {0} UTC\n", time_string);

#if (defined(_DEBUG) || !defined(NDEBUG))
    fmt::print("Kakera AVG Engine Version {0}.{1} {2}\n", VERSION_MAJOR, VERSION_MINOR, VERSION_NAME);
    fmt::print("Log create on {0} UTC\n", time_string);
#endif
}

Log::~Log()
{
    log_file.close();
}

std::string Log::now_time()
{
    std::time_t now = std::time(0);
    char buffer[32];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", std::gmtime(&now));
    return std::string(buffer);
}

void Log::get_system_info()
{
    // System info
#ifdef _WIN32
    OSVERSIONINFO osvi;
    ZeroMemory(&osvi, sizeof(OSVERSIONINFO));
    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
    GetVersionEx(&osvi);

    std::string windows_version = fmt::format("{}.{}", osvi.dwMajorVersion, osvi.dwMinorVersion);

    if (IsWindows8Point1OrGreater()) windows_version = "6.3";
    if (IsWindows10OrGreater()) windows_version = "10";

    int windows_bits = 32;
    int compile_mode = 32;

#   ifdef _WIN64
    windows_bits = 64;
    compile_mode = 64;
#   else
    if (BOOL isx64 = FALSE; IsWow64Process(GetCurrentProcess(), &isx64) && isx64)
        windows_bits = 64;
#   endif // _WIN64

    std::string system_text = fmt::format(
        "Running on Windows {} x{}, application compiled as x{} version.\n",
        windows_version, windows_bits, compile_mode);

    log_file << system_text;

#   if (defined(_DEBUG) || !defined(NDEBUG))
    fmt::print("{}", system_text);
#   endif
#else
#endif

    // Graphics card info
    std::string vendor = (char*)glGetString(GL_VENDOR);
    std::string device = (char*)glGetString(GL_RENDERER);
    std::string version = (char*)glGetString(GL_VERSION);

    std::string graphics_card_text = fmt::format(
        "Graphics card vendor: {0}\n"
        "Graphics card model: {1}\n"
        "OpenGL version: {2}\n\n",
        vendor, device, version);

    log_file << graphics_card_text;

#if (defined(_DEBUG) || !defined(NDEBUG))
    fmt::print("{}", graphics_card_text);
#endif
}

void Log::print(LogLevel level, std::string what, std::string file, int line)
{
#if (!defined(_DEBUG) || defined(NDEBUG))
    if (level == LogLevel::Debug) return;
#endif
    std::lock_guard lock(m_lock);

    char split = '/';
#ifdef _WIN32
    split = '\\';
#endif

    std::string real_file = file.substr(file.find_last_of(split) + 1);

    std::string format_str = fmt::format("[{0}] {1} {2}({3}): {4}\n", level_map[level], now_time(), real_file, line, what);
    log_file << format_str;

#if (defined(_DEBUG) || !defined(NDEBUG))
    fmt::print(format_str);
#endif

    if (level == LogLevel::Fatal) std::abort();
}

Log& Log::get()
{
    std::lock_guard lock(m_lock);
    static Log instance;
    return instance;
}
