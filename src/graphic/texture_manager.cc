#include "texture_manager.h"

void TextureManager::set_texture(std::string id, Texture&& texture)
{
    textures.try_emplace(id, std::make_unique<Texture>(std::move(texture)));
}

Texture* TextureManager::get_texture(std::string id)
{
    if (auto iter = textures.find(id); iter != textures.end())
        return iter->second.get();
    return nullptr;
}

Texture* TextureManager::operator[](std::string id)
{
    return get_texture(id);
}

void TextureManager::destroy_texture(std::string id)
{
    if (auto iter = textures.find(id); iter != textures.end())
    {
        Texture* texture = iter->second.release();
        delete texture;
        textures.erase(iter);
    }
}

TextureManager& TextureManager::get()
{
    static TextureManager instance;
    return instance;
}
