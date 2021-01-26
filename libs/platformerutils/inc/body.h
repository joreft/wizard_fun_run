#pragma once

#include "vector2.h"
#include "box.h"

#include <array>

namespace jeagle
{

struct ImmovableBody
{
    Box<float> collision_box{};

    bool solid{};
};

struct MovableBody
{
    Box<float> collision_box{};
    Vector2f speed{};

    std::array<Box<float> const*, 4> contact;

    bool on_ground {};

    int marked_as_dead{};
};

} // namespace jeagle