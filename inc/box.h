#pragma once

#include "log.h"
#include "vector2.h"

#include <json11/json11.hpp>

namespace jeagle
{

template<typename T>
struct Box
{
    Vector2 <T> upper_left;
    Vector2 <T> size;

    constexpr Vector2 <T> lower_right() const
    {
        return upper_left + size;
    }

    static Box from(json11::Json const& json)
    {
        Box box {};
        if (json["upper_left"]["x"].is_number() && json["upper_left"]["y"].is_number())
        {
            box.upper_left = Vector2i{static_cast<float>(json["upper_left"]["x"].number_value()),
                                      static_cast<float>(json["upper_left"]["y"].number_value())};
        }
        else
        {
            LOG_DEBUG("Parsing json input of non number upper left corner for box");
        }


        if (json["size"]["x"].is_number() && json["size"]["y"].is_number())
        {
            box.size = Vector2i{static_cast<float>(json["size"]["x"].number_value()),
                                static_cast<float>(json["size"]["y"].number_value())};
        }
        else
        {
            LOG_DEBUG("Parsing json input of non number size for box");
        }

        return box;
    }

    json11::Json to_json() const
    {
        json11::Json::object const upper_left_json
            {
                {"x", upper_left.x}
                , {"y", upper_left.y}
            };

        json11::Json::object const size_json
            {
                {"x", size.x}
                , {"y", size.y}
            };

        return json11::Json::object
            {
                {"upper_left", upper_left_json}
                , {"size",             size_json}
            };
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