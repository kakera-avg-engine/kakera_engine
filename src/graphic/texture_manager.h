#ifndef KAKERA_ENGINE_TEXTURE_MANAGER
#define KAKERA_ENGINE_TEXTURE_MANAGER

#include "texture.h"
#include <memory>
#include <string>
#include <unordered_map>
#include "../copy_and_move.inc"

#define KAKERA_TEXTURE_MANAGER TextureManager::get()

class TextureManager
{
private:
    std::unordered_map<std::string, std::shared_ptr<Texture>> textures;

    TextureManager() = default;
    ~TextureManager() = default;
public:
    KAKERA_DISABLE_COPY(TextureManager);
    KAKERA_DISABLE_MOVE(TextureManager);

    void set_texture(std::string id, Texture&& texture);
    std::shared_ptr<Texture> get_texture(std::string id);
    std::shared_ptr<Texture> operator[](std::string id);

    static TextureManager& get();
};

#endif // !KAKERA_ENGINE_TEXTURE_MANAGER
