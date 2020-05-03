#pragma once

#include "texture_container.h"

#include <SFML/Graphics.hpp>

#include <stdexcept>
#include <string>
#include <unordered_map>

namespace jeagle
{

struct AssetManager
{
    AssetManager(AssetManager const&) = delete;
    AssetManager(AssetManager&&) = delete;
    AssetManager operator=(AssetManager const&) = delete;
    AssetManager& operator=(AssetManager &&) = delete;

    bool ensure_texture_loaded(std::string const& texture_path);
    sf::Texture const& get_texture(std::string const& path) const;

    void ensure_animated_texture_loaded(std::string const& path);
    CreatureTextureContainer const& get_animated_texture_container(std::string const& path);

    bool ensure_font_loaded(std::string const& font_path);
    sf::Font const& get_font(std::string const& path) const;

    sf::Font const& get_default_font() const;

    static AssetManager& instance();
private:
    AssetManager();
    using Filename = std::string;

    std::unordered_map<Filename, sf::Texture> textures;
    std::unordered_map<Filename, sf::Font> fonts;
    std::unordered_map<Filename, CreatureTextureContainer> creature_textures;
};

} // namespace jeagle