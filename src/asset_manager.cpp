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

bool AssetManager::ensure_texture_loaded(std::string const& texture_path)
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

sf::Texture& AssetManager::get_texture(std::string const& path)
{
    auto const it = textures.find(path);
    if (it == textures.end())
    {
        throw AssetNotFound(fmt::format("Did not find texture '{}'", path));
    }
    return it->second;
}

static constexpr char default_font_path[] = "assets/SedgwickAve-Regular.ttf"; //"assets/Inconsolata-Bold.ttf";

sf::Font const& AssetManager::getDefaultFont()
{
    static auto const font = []()
    {
        sf::Font font;
        if (!font.loadFromFile(default_font_path))
        {
            LOG_ERROR("Failed to load default font from path {}", default_font_path);
            throw AssetNotFound("Critical error, didn't find default font");
        }

        return font;
    }();

    return font;
}

} // namespace jeagle