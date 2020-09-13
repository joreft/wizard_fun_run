#include "rectangle_collision.h"
#include "physics.h"
#include "log.h"

#include <numeric>
#include <limits>

namespace jeagle
{

void collision_resolution(PhysicsWorld& world, float s_elapsed)
{
    for (auto& movable : world.movable_bodies)
    {
        auto sections_to_check = world.get_section_indices_for_body(movable->collision_box);

        for (auto const section_idx : sections_to_check)
        {
            auto const& section = world.sections.at(section_idx);
            
            resolve_moving_rectangle_vs_world(*movable, s_elapsed, section.immovable_bodies);
        }
    }

}

void process_physics(PhysicsWorld& world, float s_elapsed)
{
    collision_resolution(world, s_elapsed);
}

} // namespace jeagle