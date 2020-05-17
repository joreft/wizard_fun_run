#pragma once

#include "box.h"
#include "log.h"
#include "scene.h"
#include "vector2.h"

#include <algorithm>
#include <vector>
#include <set>

namespace jeagle
{

struct ImmovableBody
{
    Box<float> collision_box {};
};

struct MovableBody
{
    struct State
    {
        Box<float> collision_box {};
        Vector2f speed {};

        int marked_as_dead {};
        int section_snapped_to_last_frame {};
    };

    State current_frame;
    State last_frame;
};

/**
 * Note that the physics system operates with unsigned coordinates
 *
 * And the physics system deallocates
 */
struct PhysicsWorld
{
    PhysicsWorld()
    {
        sections = std::vector<Section>(max_sections_y * max_sections_x, Section {});
    }

    // We use 16x16 tiles, so it's probably a good idea for this to not be
    // divisible by 16
    static constexpr auto section_size = 500; // physics_units

    // We have some max sizes
    static constexpr auto max_sections_x = 1000;
    static constexpr auto max_sections_y = 1000;

    // One assumption here is that a collision body is always smaller than a section
    // That is reasonable, and if it creates bugs, well, everything is my fault
    std::set<std::size_t> get_section_indices_for_body(Box<float> body)
    {
        // Using std::set because it will ignore duplicate entries, which is neat!
        std::set<std::size_t> out;

        auto point_to_section = [](float x, float y)
        {
            auto const section_x = static_cast<int>(x / static_cast<float>(section_size));
            auto const section_y = static_cast<int>(y / static_cast<float>(section_size));

            auto const section_idx = section_x + max_sections_x * section_y;

            return section_idx;
        };

        // Top left
        out.emplace(point_to_section(body.upper_left.x, body.upper_left.y));

        // Top right
        out.emplace(point_to_section(body.upper_left.x + body.size.x, body.upper_left.y));

        // Bottom right
        out.emplace(point_to_section(body.upper_left.x + body.size.x, body.upper_left.y + body.size.y));

        // Bottom left
        out.emplace(point_to_section(body.upper_left.x, body.upper_left.y + body.size.x));

        return out;
    }

    // I want each section to have a mathematical mapping
    // from world coordinates such that fetching the correct
    // section(s) is trivially easy for any movable object
    struct Section
    {
        Box<int> section_area;

        std::vector<ImmovableBody> bodies;
    };

    // A weakness here is that the same amount of memory is used regardless
    // of the size of the level
    std::vector<Section> sections {};

    /**
     * Returns a non-owning raw pointer to the physical object.
     *
     * This is used so that the game logic can both query and modify the
     * physical object according to the game rules
     */
    MovableBody* register_movable_body(MovableBody const& body)
    {
        auto ptr = std::make_unique<MovableBody>(body);
        movable_bodies.emplace_back(std::move(ptr));
        return ptr.get();
    }


    void unregister_movable_body(MovableBody* body)
    {
        body->current_frame.marked_as_dead = true;
    }

    void garbage_collect()
    {
        auto erase_from = std::remove_if(begin(movable_bodies), end(movable_bodies),
                                         [] (auto& body) {return body->current_frame.marked_as_dead;} );

        movable_bodies.erase(erase_from, end(movable_bodies));
    }

private:
    // TODO I would like these to be organised in a cache-friendly way
    // but it is hard and probably unnecessary for the scope of this project
    std::vector<std::unique_ptr<MovableBody>> movable_bodies;

};

void process_physics(PhysicsWorld& world, float s_elapsed);

} // namespace jeagle