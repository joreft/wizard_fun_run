#pragma once

#include "box.h"
#include "json11_from_file.h"
#include "player.h"
#include "texture_paths.h"
#include "log.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window.hpp>
#include <json11/json11.hpp>

#include <boost/algorithm/string.hpp>

namespace jeagle
{

struct PlayLevelCoreContextData
{
    Player player;

    // Debug info
    int fps {};
};

void start_play_level_core();

void play_level_core_handle_input_impl(sf::Event const& event, PlayLevelCoreContextData& context);
void play_level_core_update_impl(PlayLevelCoreContextData& context, float s_elapsed);
void play_level_core_draw_impl(sf::RenderWindow& window, PlayLevelCoreContextData& context);

} // namespace jeagle