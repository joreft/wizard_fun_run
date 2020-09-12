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

    int marked_as_dead{};
};

} // namespace jeagle