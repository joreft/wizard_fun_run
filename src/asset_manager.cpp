#include "asset_manager.h"
#include "fonts.h"

#include "log.h"

#include <string>
#include <utility>

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

sf::Texture const& AssetManager::get_texture(std::string const& path) const
{
    auto const it = textures.find(path);
    if (it == textures.cend())
    {
        throw AssetNotFound(fmt::format("Did not find texture '{}'", path));
    }
    return it->second;
}

void AssetManager::ensure_animated_texture_loaded(std::string const& path)
{
    creature_textures[path] = parse_creature_texture_and_metadata(path);
}

CreatureTextureContainer const& AssetManager::get_animated_texture_container(std::string const& path)
{
    auto const it = creature_textures.find(path);
    if (it == creature_textures.cend())
    {
        throw AssetNotFound(fmt::format("Did not find texture '{}'", path));
    }
    return it->second;
}

bool AssetManager::ensure_font_loaded(std::string const& font_path)
{
    if (fonts.find(font_path) != fonts.cend())
    {
        return true;
    }

    sf::Font font;

    if (!font.loadFromFile(font_path))
    {
        LOG_ERROR("Failed to load font from path {}", font_path);
        return false;
    }

    fonts[font_path] = std::move(font);
    return true;
}

sf::Font const& AssetManager::get_default_font() const
{
    return get_font(fonts::sedgwick);
}

sf::Font const& AssetManager::get_font(std::string const& path) const
{
    auto const font_at = fonts.find(path);
    if (font_at == fonts.cend())
    {
        throw AssetNotFound(fmt::format("Did not find font at '{}'", path));
    }

    return font_at->second;
}

} // namespace jeagle