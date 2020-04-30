#pragma once

#include <SFML/Graphics.hpp>

#include <stdexcept>
#include <string>
#include <unordered_map>

namespace jeagle
{

struct AssetNotFound : public std::runtime_error
{
    AssetNotFound(std::string const& s) : std::runtime_error(s)
    {}
};

struct AssetManager
{
    AssetManager(AssetManager const&) = delete;
    AssetManager(AssetManager&&) = delete;
    AssetManager operator=(AssetManager const&) = delete;
    AssetManager& operator=(AssetManager &&) = delete;

    bool ensure_texture_loaded(std::string const& texture_path);
    sf::Texture& get_texture(std::string const& path);

    sf::Font const& getDefaultFont();

    static AssetManager& instance();
private:
    AssetManager();
    using Filename = std::string;
    std::unordered_map<Filename, sf::Texture> textures;
};

} // namespace jeagle