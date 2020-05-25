#include "physics.h"

#include "log.h"

namespace jeagle
{

//void resolve_movable_on_section(Box<float>& movable, PhysicsWorld::Section const& section)
//{
//    for (auto const& tile : section.bodies)
//    {
//
//    }
//}

void collision_resolution(PhysicsWorld& world, float s_elapsed)
{
    for (auto& movable : world.movable_bodies)
    {
        auto sections_to_check = world.get_section_indices_for_body(movable->current_frame.collision_box);

        for (auto const section_idx : sections_to_check)
        {
            auto const& section = world.sections.at(section_idx);
            for (auto const& world_tile : section.immovable_bodies)
            {
                if (boxes_overlap(movable->current_frame.collision_box, world_tile.collision_box ))
                {
                    // TODO remove this log line
                    LOG_DEBUG("Found a collision here!");
                }
            }
        }


        movable->last_frame = movable->current_frame;
    }

}

void process_physics(PhysicsWorld& world, float s_elapsed)
{
    collision_resolution(world, s_elapsed);
}

} // namespace jeagle