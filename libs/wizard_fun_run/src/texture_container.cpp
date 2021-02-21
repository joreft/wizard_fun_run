#include "texture_container.h"
#include "texture_paths.h"
#include "json11_from_file.h"
#include "log.h"

#include <json11/json11.hpp>
#include <fmt/format.h>
#include <boost/algorithm/string.hpp>

namespace jeagle
{

CreatureTextureContainer parse_creature_texture_and_metadata(std::string const& texture_path)
{
    auto const metadata_path = texture_path + paths::texture_metadata_suffix;
    auto const content_path = texture_path + paths::texture_content_suffix;

    sf::Texture texture;
    if (!texture.loadFromFile(content_path))
    {
        throw AssetNotFound(fmt::format("Did not find asset at '{}'", content_path));
    }

    CreatureTextureContainer out {texture};

    if (auto const maybe_json = json11_from_file("assets/character.json"))
    {
        auto const& json = *maybe_json;
        for (auto const &val : json["frames"].array_items())
        {
            auto const filename = val["filename"].string_value();

            auto const underscore_at = std::find(filename.begin(), filename.end(), '_');
            auto const string_key = std::string(filename.begin(), underscore_at);

            auto const internal_key_at = tag_to_sequence_type_creature.find(string_key);

            if (internal_key_at == tag_to_sequence_type_creature.end())
            {
                LOG_WARNING("Did not find internal key for string key '{}'", string_key);
                continue;
            }

            auto const &frame = val["frame"];
            Box<int> box;
            box.upper_left = {frame["x"].int_value(), frame["y"].int_value()};
            box.size = {frame["w"].int_value(), frame["h"].int_value()};

            out.frame_sequences[internal_key_at->second].emplace_back(box);
        }

        return out;
    }
    else
    {
        throw AssetNotFound(fmt::format("Did not find asset metadata at '{}'", metadata_path));
    }

}

} // namespace jeagle