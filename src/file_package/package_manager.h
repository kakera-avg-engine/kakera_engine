#ifndef KAKERA_ENGINE_PACKAGE_MANAGER
#define KAKERA_ENGINE_PACKAGE_MANAGER

#include "package.h"
#include <memory>
#include <unordered_map>
#include <string>
#include <optional>
#include <list>
#include "../log/log.h"
#include "../copy_and_move.inc"

#define KAKERA_PACKAGE_MANAGER PackageManager::get()

class PackageManager
{
private:
    std::unordered_map<std::string, std::unique_ptr<Package>> packages;

    PackageManager() = default;
    ~PackageManager() = default;
public:
    KAKERA_DISABLE_COPY(PackageManager);
    KAKERA_DISABLE_MOVE(PackageManager);

    void add_package(std::string id, Package&& package);

    Package* get_package(std::string id);
    Package* operator[](std::string id);

    std::optional<File> get_file(std::string src);

    static PackageManager& get();
};

#endif // !KAKERA_ENGINE_PACKAGE_MANAGER
