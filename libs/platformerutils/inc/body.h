#pragma once

#include "vector2.h"
#include "box.h"

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

    std::array<Box<float>*, 4> contact;

    int marked_as_dead{};
};

} // namespace jeagle