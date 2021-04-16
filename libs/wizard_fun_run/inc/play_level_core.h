#pragma once

#include "box.h"
#include "json11_from_file.h"
#include "physics.h"
#include "player.h"
#include "texture_paths.h"
#include "scene.h"
#include "log.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window.hpp>
#include <json11/json11.hpp>

#include <boost/algorithm/string.hpp>

namespace jeagle
{

struct Projectile
{
    Box<int> collision {};

    Vector2f speed {};

    float time_accumulator {};
    int frame {};
};

struct KeyInputState
{
    bool left_pressed {};
    bool right_pressed {};
};

struct PlayLevelCoreContextData
{
    Player player {};

    PhysicsWorld physics_world {};
    Scene scene = Scene("assets/levels/level1.json", &physics_world);
    std::vector<Projectile> projectiles {};

    KeyInputState key_input_state {};

    // Debug info
    int fps {};
};

void start_play_level_core();

/**
 * This is mostly used for populating input structures
 *
 * Note that it should NOT change the state of the physical game directly
 *
 * but rather indirectly through messages to the update method. E.g. handle_input
 * will say that the left and/or right button is pressed, but it will not decide
 * which direction the player should move.
 */
void play_level_core_handle_input_impl(sf::Event const& event, PlayLevelCoreContextData& context);

/**
 * The game logic happens here!
 *
 * So this is a big one.
 */
void play_level_core_update_impl(PlayLevelCoreContextData& context, float s_elapsed);

/**
 * The function name is probably self sufficient. But this handles rendering to the screen.
 *
 * It may also be used for minimap logic or similar at some point
 */
void play_level_core_draw_impl(sf::RenderWindow& window, PlayLevelCoreContextData& context);

} // namespace jeagle