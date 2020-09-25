#include "rectangle_collision.h"
#include "physics.h"
#include "log.h"

#include <numeric>
#include <limits>

namespace jeagle
{

static constexpr auto gravity = 800.f;

static void apply_gravity(MovableBody& movable, float s_elapsed)
{
    movable.speed += Vector2f{0, gravity}*s_elapsed;
}



void collision_resolution(PhysicsWorld& world, float s_elapsed)
{
    for (auto& movable : world.movable_bodies)
    {
        movable->on_ground = false;
        apply_gravity(*movable, s_elapsed);
        auto sections_to_check = world.get_section_indices_for_body(movable->collision_box);

        for (auto const section_idx : sections_to_check)
        {
            auto const& section = world.sections.at(section_idx);

            resolve_moving_rectangle_vs_world(*movable, s_elapsed, section.immovable_bodies);
        }

        movable->collision_box.upper_left += movable->speed * s_elapsed;

        if (movable->on_ground)
        {
            LOG_DEBUG("On ground");
        }
    }
}

void process_physics(PhysicsWorld& world, float s_elapsed)
{
    collision_resolution(world, s_elapsed);
}

} // namespace jeagle