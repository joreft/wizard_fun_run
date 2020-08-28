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

    // At the end of every frame this should
    // be set to denote the section at which the collision box is in (upper left)
    int was_at_section{};
//    State current_frame;
};

} // namespace jeagle