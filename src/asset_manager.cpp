#include "asset_manager.h"

#include "log.h"

#include <string>

namespace jeagle
{

AssetManager::AssetManager()
{}

AssetManager& AssetManager::instance()
{
    static AssetManager asset_manager;
    return asset_manager;
}

bool AssetManager::ensure_loaded(std::string const& texture_path)
{
    if (textures.find(texture_path) != textures.end())
    {
        return true;
    }
    else
    {
        sf::Texture texture;
        if (!texture.loadFromFile(texture_path))
        {
            LOG_DEBUG("Failed to load texture from file '{}'", texture_path);
            return false;
        }
        textures[texture_path] = std::move(texture);
        LOG_DEBUG("Loaded texture '{}'", texture_path);
        return true;
    }

}

sf::Texture& AssetManager::get(std::string const& path)
{
    auto const it = textures.find(path);
    if (it == textures.end())
    {
        throw AssetNotFound(fmt::format("Did not find texture '{}'", path));
    }
    return it->second;
}

} // namespace jeagle