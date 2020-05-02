#pragma once

#include "box.h"
#include "conversion.h"

#include <json11/json11.hpp>

#include <SFML/Graphics.hpp>

#include <string>
#include <unordered_map>
#include <vector>

namespace jeagle
{

struct AnimatedTextureContainer
{
    sf::Texture texture {};

    using KeyType = int;
    std::unordered_map<KeyType, std::vector<Box<int>>> frame_sequences {};

    template <typename SequenceKey>
    sf::Sprite get_as_sprite(SequenceKey key, int number_in_frame, Vector2i const& in_position)
    {
        sf::Sprite sprite;
        sprite.setTexture(texture);

        sprite.setTextureRect(int_rect_from_box(frame_sequences.at(to_integral(key)).at(number_in_frame)));
        sprite.setPosition(in_position.x, in_position.y);
        return sprite;
    }
};

enum class PlayerSequences
{
      idle
    , casting_swing
    , casting_backswing
    , walking
    , jumping
};

}