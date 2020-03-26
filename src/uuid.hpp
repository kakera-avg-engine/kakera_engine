#ifndef KAKERA_ENGINE_UUID_GENERATOR
#define KAKERA_ENGINE_UUID_GENERATOR

#include <string>
#ifdef _WIN32
#   include <objbase.h>
#else
#endif // _WIN32

namespace kakera
{
    class UUID
    {
    public:
        static std::string create()
        {
            std::string result;
#ifdef _WIN32
            GUID guid;
            HRESULT r = CoCreateGuid(&guid);
            if (r == S_OK) {
                char buffer[37];
                snprintf(buffer, sizeof(buffer), "%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x",
                    guid.Data1, guid.Data2, guid.Data3, guid.Data4[0], guid.Data4[1], guid.Data4[2],
                    guid.Data4[3], guid.Data4[4], guid.Data4[5], guid.Data4[6], guid.Data4[7]);
                result = std::string(buffer);
            }
#else
#endif // _WIN32
            return result;
        }
    };
}

#endif // !KAKERA_ENGINE_UUID_GENERATOR
