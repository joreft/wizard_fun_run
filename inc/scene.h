#pragma once

#include "box.h"
#include "physics.h"

#include <json11/json11.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include <map>
#include <optional>
#include <stdexcept>

namespace jeagle
{

struct SceneCreationException : public std::runtime_error
{
    SceneCreationException(std::string const& s);
};

// TODO invert the logic of this
// Should be
// using Tile = std::variant<StaticCollisionTile, CosmeticTile, BehaviorTile etc.>;
struct Tile
{
    std::string tileset_path;
    Box<int> tileset_position;
    Box<int> sprite_box;
    std::optional<Box<int>> collision_box;

    static Tile from(json11::Json const& json);

    json11::Json to_json() const;
};

void add_to_vertex_array_quads(Box<int> const& position_in_sheet, Box<int> const& physical_position, sf::VertexArray& vertex_array);

/**
 * We use this trick to only do one draw call for each texture in the scene
 * instead of doing it for every sprite
 */
struct DrawableBuffer : public sf::Drawable
{
    DrawableBuffer(sf::Texture const& texture);

    sf::VertexArray vertex_array = sf::VertexArray(sf::Quads, 0);
    sf::Texture const& texture;

    void add(Box<int> const& position_in_sheet, Box<int> const& physical_position);
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

struct Scene
{
public:
    Scene();
    Scene(std::string const& full_path, jeagle::PhysicsWorld* physics = nullptr);

    static Scene create_empty(std::string const& file_path);
    void draw(sf::RenderWindow& window);

    void add_tile(Box<int> const& sprite_box, std::optional<Box<int>> const& collision_box,
                  std::string const& tileset_path, Box<int> const& position_in_texture);

    json11::Json to_json() const;
    void export_to_file(std::string const& path);
    void save();
    void erase();

private:
    std::vector<Tile> tiles;
    std::string scene_path;

    std::map<std::string, DrawableBuffer> drawables;
    PhysicsWorld* physics;

private:
    void add_to_drawables(Tile const& tile, sf::Texture const& texture);
    void add_collision_box(Tile const& tile);
};

} // namespace jeagle