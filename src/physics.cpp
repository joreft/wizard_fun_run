#include "physics.h"

#include "log.h"

#include <numeric>
#include <limits>

namespace jeagle
{

//void resolve_movable_on_section(Box<float>& movable, PhysicsWorld::Section const& section)
//{
//    for (auto const& tile : section.bodies)
//    {
//
//    }
//}

float swept_aabb(MovableBody& movable, Box<float> const& static_box, Vector2f& normal)
{
    float xInvEntry, yInvEntry;
    float xInvExit, yInvExit;

    auto const& movable_box = movable.current_frame.collision_box;

    if (movable.current_frame.speed.x > 0.0f)
    {
        xInvEntry = static_box.upper_left.x - (movable_box.upper_left.x + movable_box.size.x);
        xInvExit = (static_box.upper_left.x + static_box.size.x) - movable_box.upper_left.x;
    }
    else
    {
        xInvEntry = (static_box.upper_left.x + static_box.size.x) - movable_box.upper_left.x;
        xInvExit = static_box.upper_left.x - (movable_box.upper_left.x + movable_box.size.x);
    }

    if (movable.current_frame.speed.y > 0.0f)
    {
        yInvEntry = static_box.upper_left.y - (movable_box.upper_left.y + movable_box.size.y);
        yInvExit = (static_box.upper_left.y + static_box.size.y) - movable_box.upper_left.y;
    }
    else
    {
        yInvEntry = (static_box.upper_left.y + static_box.size.y) - movable_box.upper_left.y;
        yInvExit = static_box.upper_left.x - (movable_box.upper_left.x + movable_box.size.x);
    }

    float xEntry, yEntry;
    float xExit, yExit;

    if (movable.current_frame.speed.x == 0.0f)
    {
        xEntry = -std::numeric_limits<float>::infinity();
        xExit = std::numeric_limits<float>::infinity();
    }
    else
    {
        xEntry = xInvEntry / movable.current_frame.speed.x;
        xExit = xInvExit / movable.current_frame.speed.x;
    }

    if (movable.current_frame.speed.y == 0.0f)
    {
        yEntry = -std::numeric_limits<float>::infinity();
        yExit = std::numeric_limits<float>::infinity();
    }
    else
    {
        yEntry = yInvEntry / movable.current_frame.speed.y;
        yExit = yInvExit / movable.current_frame.speed.y;
    }

    // find the earliest/latest times of collisionfloat
    float entryTime = std::max(xEntry, yEntry);
    float exitTime = std::min(xExit, yExit);

    // if there was no collision
    if ((entryTime > exitTime) || ((xEntry < 0.0f) && (yEntry < 0.0f)) || (xEntry > 1.0f) || (yEntry > 1.0f))
    { LOG_DEBUG("No collision");
        normal.x = 0.0f;
        normal.y = 0.0f;
        return 1.0f;
    }

    else // if there was a collision
    {
        LOG_DEBUG("COLLISION");
        // calculate normal of collided surface
        if (xEntry > yEntry)
        {
            if (xInvEntry < 0.0f)
            {
                normal.x = 1.0f;
                normal.y = 0.0f;
            }
            else
            {
                normal.x = -1.0f;
                normal.y = 0.0f;
            }
        }
        else
        {
            if (yInvEntry < 0.0f)
            {
                normal.x = 0.0f;
                normal.y = 1.0f;
            }
            else
            {
                normal.x = 0.0f;
                normal.y = -1.0f;
            }
        } // return the time of collisionreturn entryTime;
    }

    return entryTime;
}

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
                Vector2f normal;
                auto time = swept_aabb(*movable, world_tile.collision_box, normal);
                if (time < 1)
                {
                    LOG_DEBUG("Oh shit fell into something: {}", time);
                }

//                movable->current_frame.collision_box.upper_left.x += movable->current_frame.speed.x * time;
//                movable->current_frame.collision_box.upper_left.y += movable->current_frame.speed.y * time;
                float remainingtime = 1.0f - time;

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


        movable->last_frame = movable->current_frame;
    }

}

void process_physics(PhysicsWorld& world, float s_elapsed)
{
    collision_resolution(world, s_elapsed);
}

} // namespace jeagle