#pragma once

#include "vector2.h"

namespace jeagle
{

template<typename T>
struct Box
{
    constexpr Box(Vector2 <T> const &upper_left, Vector2 <T> const &size)
        : upper_left(upper_left), size(size)
    {}

    Vector2 <T> upper_left;
    Vector2 <T> size;

    constexpr Vector2 <T> lower_right() const
    {
        return upper_left + size;
    }
};

template<typename T>
constexpr bool point_inside_box(Vector2 <T> const& point, Box<T> const &box)
{
    auto const within_x = (point.x > box.upper_left.x) && (point.x < box.lower_right().x);
    auto const within_y = (point.y > box.upper_left.y) && (point.y < box.lower_right().y);
    return within_x &&within_y;
}

static_assert(point_inside_box(Vector2 <int> {1, 1}, Box <int> {{0, 0}, {2, 2}}), "Of course it should work");
static_assert(!point_inside_box(Vector2 <int> {1, 1}, Box <int> {{0, 0}, {1, 2}}), "On the bound is not within");
static_assert(!point_inside_box(Vector2 <int> {5, 1}, Box <int> {{0, 0}, {1, 2}}), "Long outside in x axis");
static_assert(!point_inside_box(Vector2 <int> {1, 5}, Box <int> {{0, 0}, {1, 2}}), "Long outside in y axis");

} // namespace jeagle