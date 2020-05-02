#pragma once

#include <SFML/Graphics.hpp>

#include <type_traits>

namespace jeagle
{

template <typename Enum>
std::underlying_type<Enum> to_integral(Enum const val)
{
    return static_cast<std::underlying_type<Enum>>(val);
}

template <typename T>
inline sf::IntRect int_rect_from_box(Box<T> const& box)
{
    return {box.upper_left.x, box.upper_left.y, box.size.x, box.size.y};
}

} // namespace jeagle