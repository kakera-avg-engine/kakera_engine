#include "package_manager.h"

void PackageManager::add_package(std::string id, Package&& package)
{
    packages.try_emplace(id, std::make_unique<Package>(std::move(package)));
}

Package* PackageManager::get_package(std::string id)
{
    if (auto iter = packages.find(id); iter != packages.end())
        return iter->second.get();
    return nullptr;
}

Package* PackageManager::operator[](std::string id)
{
    return get_package(id);
}

std::optional<File> PackageManager::get_file(std::string src)
{
    if (src[0] != '{') {
        KAKERA_LOG(LogLevel::Error, "Given path \"{}\" is not a Kakera format package path.", src);
        return std::nullopt;
    }

    std::string::size_type pos = src.find('}');

    if (pos == std::string::npos) {
        KAKERA_LOG(LogLevel::Error, "Given path \"{}\" is not a Kakera format package path.", src);
        return std::nullopt;
    }

    std::string package_name = src.substr(1, pos - 1);
    std::string file_path = src.substr(pos + 2);

    Package* package = get_package(package_name);
    if (!package) {
        KAKERA_LOG(LogLevel::Error, "No such package \"{}\" found.", package_name);
        return std::nullopt;
    }

    return package->get_file(file_path);
}

PackageManager& PackageManager::get()
{
    static PackageManager instance;
    return instance;
}
