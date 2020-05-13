#pragma once

#include "box.h"
#include "json11_from_file.h"
#include "player.h"
#include "texture_paths.h"
#include "level_tiles.h"
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

struct PlayLevelCoreContextData
{
    Player player;
    Scene scene = Scene(std::string("assets/levels/level1.json"));

    std::vector<Projectile> projectiles;

    // Debug info
    int fps {};
};

struct KeyInputState
{
    bool left_pressed {};
    bool right_pressed {};
    bool jump_pressed {};
};

void start_play_level_core();

void play_level_core_handle_input_impl(sf::Event const& event, PlayLevelCoreContextData& context, KeyInputState& key_input_state);
void play_level_core_update_impl(PlayLevelCoreContextData& context, float s_elapsed);
void play_level_core_draw_impl(sf::RenderWindow& window, PlayLevelCoreContextData& context);

} // namespace jeagle