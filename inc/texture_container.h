#pragma once

#include "box.h"
#include "conversion.h"

#include <json11/json11.hpp>

#include <SFML/Graphics.hpp>

#include <string>
#include <type_traits>
#include <unordered_map>
#include <vector>

namespace jeagle
{

struct AssetNotFound : public std::runtime_error
{
    AssetNotFound(std::string const& s) : std::runtime_error(s)
    {}
};

enum class CreatureSequence
{
    idle
    , casting_swing
    , casting_backswing
    , walking
    , jumping
};

struct CreatureTextureContainer
{
    sf::Texture texture {};

    std::unordered_map<CreatureSequence, std::vector<Box<int>>> frame_sequences {};

    template <typename SequenceKey>
    sf::Sprite get_as_sprite(SequenceKey key, int number_in_frame, Vector2i const& in_position)
    {
        static_assert(std::is_same<std::underlying_type<SequenceKey>, CreatureSequence>::value);
        sf::Sprite sprite;
        sprite.setTexture(texture);

        sprite.setTextureRect(int_rect_from_box(frame_sequences.at(to_integral(key)).at(number_in_frame)));
        sprite.setPosition(in_position.x, in_position.y);
        return sprite;
    }
};

inline std::unordered_map<std::string, CreatureSequence> tag_to_sequence_type_creature
{
      {"idle",               CreatureSequence::idle}
    , {"casting_swing",      CreatureSequence::casting_swing}
    , {"casting_backswing",  CreatureSequence::casting_backswing}
    , {"walking",            CreatureSequence::walking}
    , {"jumping",            CreatureSequence::jumping}
};

CreatureTextureContainer parse_creature_texture_and_metadata(std::string const& texture_path);

} // namespace jeagle