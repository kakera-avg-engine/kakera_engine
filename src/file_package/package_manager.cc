#include "package_manager.h"

void PackageManager::add_package(std::string id, Package&& package)
{
    packages.try_emplace(id, std::make_shared<Package>(package));
}

std::shared_ptr<Package> PackageManager::get_package(std::string id)
{
    if (auto iter = packages.find(id); iter != packages.end()) return iter->second;
    return std::shared_ptr<Package>(nullptr);
}

std::shared_ptr<Package> PackageManager::operator[](std::string id)
{
    return get_package(id);
}

PackageManager& PackageManager::get()
{
    static PackageManager instance;
    return instance;
}
