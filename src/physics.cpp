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
            for (auto const& world_tile : section.immovable_bodies)
            {
                Vector2f normal;
//                auto time = swept_aabb(*movable, world_tile.collision_box, normal);

                float contact_time {};
                Vector2<float> contact {1, 2};
                Vector2<float> contact_normal {0, 0};
                if (moving_rectangle_vs_rectangle(*movable, world_tile.collision_box, contact, contact_normal, contact_time
                    , s_elapsed))
                {
                    LOG_DEBUG("We have a collision over here");
                    LOG_DEBUG("time: {}\nnormal: ({},{})", contact_time, contact_normal.x, contact_normal.y);
                }
//                if (time < 1)
//                {
//                    LOG_DEBUG("Oh shit fell into something: {}", time);
//                }

//                movable->current_frame.collision_box.upper_left.x += movable->current_frame.speed.x * time;
//                movable->current_frame.collision_box.upper_left.y += movable->current_frame.speed.y * time;
//                float remainingtime = 1.0f - time;

//                if (boxes_overlap(movable->current_frame.collision_box, world_tile.collision_box ))
//                {
//                    // TODO remove this log line
//                    //LOG_DEBUG("Found a collision here!");
//                    Vector2f normal;
//                    auto time = swept_aabb(*movable, world_tile.collision_box, normal);
//                    if (time > 0)
//                    {
//                        LOG_DEBUG("Oh shit fell into something: {}", time);
//                    }
//
//                    movable->current_frame.collision_box.upper_left.x += movable->current_frame.speed.x * time;
//                    movable->current_frame.collision_box.upper_left.y += movable->current_frame.speed.y * time;
//                    float remainingtime = 1.0f - time;
//
//
//                }
            }
        }
    }

}

void process_physics(PhysicsWorld& world, float s_elapsed)
{
    collision_resolution(world, s_elapsed);
}

} // namespace jeagle