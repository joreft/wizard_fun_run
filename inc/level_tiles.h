#pragma once

#pragma once

#include "box.h"
#include "asset_manager.h"

#include <json11/json11.hpp>
#include <SFML/Window.hpp>

#include <fstream>
#include <optional>
#include <sstream>
#include <stdexcept>

namespace jeagle
{

struct SceneCreationException : public std::runtime_error
{
    SceneCreationException(std::string const& s) : std::runtime_error(s)
    {}
};

struct Tile
{
    std::string tileset_path;
    Box<int> tileset_position;
    Box<int> sprite_box;
    std::optional<Box<int>> collision_box;

    static Tile from(json11::Json const& json)
    {
        Tile tile {};
//        if (json["tileset_path"].is_string())
//        {
//            tile.tileset_path = json["tileset_path"].string_value();
//        }
//
//        if (json["tileset_box"].is_object())
//        {
//            tile.tileset_position = Box::from(json["tileset_box"]);
//        }
//
//        if (json["collision_box"].is_object())
//        {
//            tile.collision_box = Box::from(json["collision_box"]);
//        }
//
//        if (json["sprite_box"].is_object())
//        {
//            tile.sprite_box = Box::from(json["sprite_box"]);
//        }

        return tile;
    }

    json11::Json to_json() const
    {
        json11::Json::object obj
            {
//                {"tileset_path", tileset_path}
//                , {"tileset_box", json11::Json(tileset_position)}
//                , {"sprite_box", json11::Json(sprite_box)}
            };
//        if (collision_box)
//        {
//            obj["collision_box"] = json11::Json(*collision_box);
//        }

        return obj;
    }
};

inline void add_to_vertex_array_quads(Box<int> const& position_in_sheet, Box<int> const& physical_position, sf::VertexArray& vertex_array)
{
    sf::Vertex vertex1(sf::Vector2f(physical_position.upper_left.x,  physical_position.upper_left.y),
                       sf::Vector2f(position_in_sheet.upper_left.x,   position_in_sheet.upper_left.y));

    sf::Vertex vertex2(sf::Vector2f(physical_position.upper_left.x + physical_position.size.x,  physical_position.upper_left.y),
                       sf::Vector2f(position_in_sheet.upper_left.x + position_in_sheet.size.x, position_in_sheet.upper_left.y));

    sf::Vertex vertex3(sf::Vector2f(physical_position.upper_left.x + physical_position.size.x,  physical_position.upper_left.y + physical_position.size.y),
                       sf::Vector2f(position_in_sheet.upper_left.x + position_in_sheet.size.x, position_in_sheet.upper_left.y + position_in_sheet.size.y));

    sf::Vertex vertex4(sf::Vector2f(physical_position.upper_left.x,  physical_position.upper_left.y + physical_position.size.y),
                       sf::Vector2f(position_in_sheet.upper_left.x,   position_in_sheet.upper_left.y + position_in_sheet.size.y));

    vertex_array.append(vertex1);
    vertex_array.append(vertex2);
    vertex_array.append(vertex3);
    vertex_array.append(vertex4);
}

/**
 * I don't want to use that many draw calls...
 */
struct DrawableBuffer : public sf::Drawable
{
    DrawableBuffer(sf::Texture const& texture) : texture(texture)
    {}

    sf::VertexArray vertex_array = sf::VertexArray(sf::Quads, 0);
    sf::Texture const& texture;

    void add(Box<int> const& position_in_sheet, Box<int> const& physical_position)
    {
        add_to_vertex_array_quads(position_in_sheet, physical_position, vertex_array);
    }
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override
    {
        states.texture = &texture;
        target.draw(vertex_array, states);
    }
};

struct Scene
{
public:
    Scene(){}
    Scene(std::string const& full_path) : scene_path(full_path)
    {
        std::ifstream level(std::string(full_path).c_str()); // fuck this
        std::stringstream buffer;
        buffer << level.rdbuf();
        std::string e;
        auto const scene_json = json11::Json::parse(buffer.str(), e);

        if (!e.empty())
        {
            throw SceneCreationException(fmt::format("Unable to parse json from scene with '{}'", e));
        }


        for (auto const& tile : scene_json["tiles"].array_items())
        {
            auto tile_value = Tile::from(tile);

            AssetManager::instance().ensure_texture_loaded(tile_value.tileset_path);

            add_to_drawables(tile_value, AssetManager::instance().get_texture(tile_value.tileset_path)); // Let it throw from main for now

            tiles.emplace_back(std::move(tile_value));
            add_collision_box(tiles.at(tiles.size() - 1));
        }
    }

    static Scene CreateEmptyScene(std::string const& file_path)
    {
        Scene scene{};
        scene.scene_path = file_path;
        return scene;
    }

    void refill_drawables()
    {
        drawables = {};
        for (auto const& tile : tiles)
        {
            add_to_drawables(tile, AssetManager::instance().get_texture(tile.tileset_path));
        }
    }

    void draw(sf::RenderWindow& window)
    {
        for (auto& value : drawables)
        {
            auto& drawable = value.second;
            window.draw(drawable);
        }
    }

    void add_tile(Box<int> const& sprite_box, std::optional<Box<int>> const& collision_box, std::string const& tileset_path, Box<int> const& position_in_texture)
    {
        Tile tile{tileset_path, position_in_texture, sprite_box, collision_box};
        //add_to_drawables(tile, AssetManager::instance().get(tile.tileset_path)); TODO
        tiles.emplace_back(std::move(tile));
    }

    json11::Json to_json() const
    {
        json11::Json::array arr {};
        for (auto const& tile : tiles)
        {
            arr.emplace_back(json11::Json(tile));
        }

        return json11::Json::object
            {
                {"tiles", arr}
            };
    }

    void export_to_file(std::string const& path)
    {
        auto const as_json = to_json();
        auto const as_string = as_json.dump();
        std::ofstream file(path.c_str());

        file << as_string << std::endl;
    }

    void save()
    {
        export_to_file(scene_path);
    }

    void erase()
    {
        *this = CreateEmptyScene(scene_path);
    }

private:
    std::vector<Tile> tiles;
    std::string scene_path;

    std::map<std::string, DrawableBuffer> drawables;

    //std::vector<b2PolygonShape> boxes;

private:
    void add_to_drawables(Tile const& tile, sf::Texture const& texture)
    {
        if (drawables.find(tile.tileset_path) == drawables.end())
        {
            drawables.emplace(std::pair(tile.tileset_path, DrawableBuffer(texture)));
        }
        auto const it = drawables.find(tile.tileset_path); // TODO dirty stuff as there's already been a search
        if (it == drawables.end()) LOG_DEBUG("WTF");
        it->second.add(tile.tileset_position, tile.sprite_box);
    }

    void add_collision_box(Tile const& tile)
    {
        if (tile.collision_box)
        {
        }
    }
};

} // namespace jeagle