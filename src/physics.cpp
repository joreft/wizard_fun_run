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

            std::vector<Box<float>> bodies;
            for (auto const& body : section.immovable_bodies)
            {
                bodies.push_back(body.collision_box);
            }

            resolve_moving_rectangle_vs_world(*movable, s_elapsed, bodies);

//            for (auto const& world_tile : section.immovable_bodies)
//            {
//
//                float contact_time {};
//                Vector2<float> contact {};
//                Vector2<float> contact_normal {};
//                if (moving_rectangle_vs_rectangle(*movable, world_tile.collision_box, contact, contact_normal, contact_time
//                    , s_elapsed))
//                {
//                    movable->speed = {};
//                }
//            }
        }
    }

}

void process_physics(PhysicsWorld& world, float s_elapsed)
{
    collision_resolution(world, s_elapsed);
}

} // namespace jeagle