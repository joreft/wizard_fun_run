#pragma once
#include "vector2.h"
#include "box.h"
#include "body.h"
#include <algorithm>
#include <cstdint>

/**
* Heavily inspired by https://www.youtube.com/watch?v=8JJ-4JgR7Dg
*/

namespace jeagle
{

template<typename T>
constexpr bool ray_intersects_rectangle(Vector2<T> const& ray_origin, Vector2<T> const& ray_dir, Box<T> const& target,
                                        Vector2<T>& contact_point, Vector2<T>& contact_normal, T& t_hit_near)
{
    contact_normal = {};
    contact_point = {};

    // Cache division
    auto const invdir = Vector2<float> {1.f / ray_dir.x, 1.f/ray_dir.y};

    auto t_near = Vector2<T>{(target.upper_left.x - ray_origin.x)  * invdir.x,
                             (target.upper_left.y - ray_origin.y) * invdir.y};

    auto t_far = Vector2<T>{(target.upper_left.x + target.size.x - ray_origin.x) * invdir.x,
                                  (target.upper_left.y + target.size.y - ray_origin.y) * invdir.y};

    if (std::isnan(t_far.y) || std::isnan(t_far.x)) return false;
    if (std::isnan(t_near.y) || std::isnan(t_near.x)) return false;

    if (t_near.x > t_far.x) std::swap(t_near.x, t_far.x);
    if (t_near.y > t_far.y) std::swap(t_near.y, t_far.y);

    if ((t_near.x > t_far.y) || (t_near.y > t_far.x)) return false;

    t_hit_near = std::max(t_near.x, t_near.y);
    auto t_hit_far = std::min(t_far.x, t_far.y);

    if (t_hit_far < 0)
    {
        return false;
    }

    contact_point = ray_origin + t_hit_near * ray_dir;

    if (t_near.x > t_near.y)
    {
        if (ray_dir.x < 0)
        {
            contact_normal = {1, 0};
        }
        else
        {
            contact_normal = {-1, 0};
        }
    }
    else if (t_near.x < t_near.y)
    {
        if (ray_dir.y < 0)
        {
            contact_normal = {0, 1};
        }
        else
        {
            contact_normal = {0, -1};
        }
    }

    return true;
}

constexpr bool ray_intersects_rectangle_test()
{
    float t_hit_near {};
    Vector2<float> contact {1, 2};
    Vector2<float> contact_normal {0, 0};
    return ray_intersects_rectangle(Vector2<float>{1, 2},
                                    Vector2<float>{3, 1}, Box <float> {{0, 0}, {2, 2}},
                                    contact, contact_normal, t_hit_near);
}

constexpr bool moving_rectangle_vs_rectangle(MovableBody const& in, Box<float> const& target,
                                             Vector2f& contact_point, Vector2f& contact_normal, float& contact_time,
                                             float const elapsed_time)
{
    if (in.speed.x == 0 && in.speed.y == 0)
    {
        // assumption that if it isn't moving it didn't collide
        return false;
    }

    Box<float> expanded_target{(target.upper_left - in.collision_box.size / 2.f),
                               (target.size + in.collision_box.size)};

    if (ray_intersects_rectangle(in.collision_box.upper_left + in.collision_box.size / 2.f,
                                in.speed * elapsed_time, expanded_target, contact_point, contact_normal, contact_time))
    {
        return ((contact_time >= 0.0f) && (contact_time < 1.0f));
    }

    return false;
}

bool resolve_moving_rectangle_vs_rectangle(MovableBody& dynamic, const float fTimeStep, Box<float> const& r_static)
{
    Vector2f contact_point {};
    Vector2f contact_normal {};
    float contact_time = 0.0f;
    if (moving_rectangle_vs_rectangle(dynamic, r_static, contact_point, contact_normal, contact_time, fTimeStep))
    {
//        if (contact_normal.y > 0) r_dynamic->contact[0] = r_static; else nullptr;
//        if (contact_normal.x < 0) r_dynamic->contact[1] = r_static; else nullptr;
//        if (contact_normal.y < 0) r_dynamic->contact[2] = r_static; else nullptr;
//        if (contact_normal.x > 0) r_dynamic->contact[3] = r_static; else nullptr;
//
        dynamic.speed += Vector2f{contact_normal.x * std::abs(dynamic.speed.x), contact_normal.y * std::abs(dynamic.speed.y)} *
                          (1 - contact_time);
        return true;
    }

    return false;
}

void resolve_moving_rectangle_vs_world(MovableBody& dynamic, float const time_step, std::vector<ImmovableBody> const& world_tiles)
{

    std::vector<std::pair<int, float>> collisions;

    for (std::size_t i = 0; i < world_tiles.size(); ++i)
    {
        Vector2f contact_normal {};
        Vector2f contact_point {};
        float t {};


        if (moving_rectangle_vs_rectangle(dynamic, world_tiles[i].collision_box, contact_point, contact_normal, t, time_step))
        {
            collisions.push_back({i, t});
        }
    }

    std::sort(collisions.begin(), collisions.end(), [](const std::pair<int, float>& a, const std::pair<int, float>& b)
    {
        return a.second < b.second;
    });

    // Now resolve the collision in correct order
    for (auto j : collisions)
        resolve_moving_rectangle_vs_rectangle(dynamic, time_step, world_tiles[j.first].collision_box);
}

//bool maybe_resolve_dynamic_vs_static_rectangle()
//{}

static_assert(ray_intersects_rectangle_test() == true);

} // namespace jeagle