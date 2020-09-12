#pragma once

#include "vector2.h"

#include "fmt/format.h"

#include <json11/json11.hpp>
#include <string>

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
            box.upper_left = Vector2i{json["upper_left"]["x"].int_value(),
                                      json["upper_left"]["y"].int_value()};
        }
        else
        {
            //LOG_DEBUG("Parsing json input of non number upper left corner for box");
        }


        if (json["size"]["x"].is_number() && json["size"]["y"].is_number())
        {
            box.size = Vector2i{json["size"]["x"].int_value(),
                                json["size"]["y"].int_value()};
        }
        else
        {
            //LOG_DEBUG("Parsing json input of non number size for box");
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

    std::string print() const
    {
        return fmt::format("upper_left:[x:{}, y:{}],  size:[x:{}, y:{}]", upper_left.x, upper_left.y, size.x, size.y);
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


template <typename T>
bool constexpr lines_overlap(T line_1_start, T line_1_end, T line_2_start, T line_2_end)
{
    return (line_1_start < line_2_end) && (line_2_start < line_1_end);
}

static_assert(lines_overlap(0, 3, 1, 4) == true);
static_assert(lines_overlap(0, 3, -2, 2) == true);
static_assert(lines_overlap(0, 4, 1, 2) == true);
static_assert(lines_overlap(1, 2, 0, 4) == true);
static_assert(lines_overlap(5, 6, 0, 4) == false);

template <typename T>
constexpr bool boxes_overlap(Box<T> const& lhs, Box<T> const& rhs)
{

    auto const lhs_start_x = lhs.upper_left.x;
    auto const lhs_end_x = lhs.upper_left.x + lhs.size.x;

    auto const rhs_start_x = rhs.upper_left.x;
    auto const rhs_end_x = rhs.upper_left.x + rhs.size.x;

    auto const lhs_start_y = lhs.upper_left.y;
    auto const lhs_end_y = lhs.upper_left.y + lhs.size.y;
    auto const rhs_start_y = rhs.upper_left.y;
    auto const rhs_end_y = rhs.upper_left.y + rhs.size.y;

    bool const y_overlap = lines_overlap(lhs_start_y, lhs_end_y, rhs_start_y, rhs_end_y);
    bool const x_overlap = lines_overlap(lhs_start_x, lhs_end_x, rhs_start_x, rhs_end_x);

    return x_overlap && y_overlap;
}

} // namespace jeagle