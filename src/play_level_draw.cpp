#include "play_level_core.h"

#include "asset_manager.h"
#include "conversion.h"
#include "fonts.h"
#include "level_tiles.h"

#include <fmt/format.h>

namespace jeagle
{

std::string stringify_state(Player::State state)
{
    switch (state)
    {
        case Player::State::casting_swing:
            return "casting_swing";
        case Player::State::idle:
            return "idle";
    }

    return "unknown";
}

#ifndef NDEBUG
static void draw_debug_info(sf::RenderWindow& window, sf::View view, Player const& player, int fps)
{
    using namespace fmt;
    std::vector<std::string> const debug_info
    {
        format("FPS: {}", fps),
        format("Position, x: {:.2f}, y: {:.2f}", player.position.x, player.position.y),
        format("Speed, x: {:.2f}, y: {:.2f}", player.speed.x, player.speed.y),
        format("Player state: {}", stringify_state(player.state))
        //format("Acceleration, x: {:.2f}, y: {:.2f}", player_newtonian->acceleration.x, player_newtonian->acceleration.y)
    };

    auto const text_size = 14;
    auto current_position = 0;
    sf::RectangleShape text_background;
    text_background.setPosition(view.getCenter().x - view.getSize().x / 2, view.getCenter().y - view.getSize().y / 2);
    text_background.setFillColor(sf::Color{255, 255, 255, 100});
    text_background.setSize({250, static_cast<float>(debug_info.size() * text_size + 5)});

    window.draw(text_background);
    for (auto const& str : debug_info)
    {
        sf::Text text;
        text.setString(str);
        text.setCharacterSize(text_size);
        text.setFont(AssetManager::instance().get_font(fonts::inconsolata));
        text.setPosition({10 + view.getCenter().x - view.getSize().x / 2, view.getCenter().y - view.getSize().y / 2 + current_position});
        text.setFillColor(sf::Color::Black);
        //text.setScale(640.f/1920.f, 640.f/1920.f);
        current_position += text_size;
        window.draw(text);
    }
}
#endif

void play_level_core_draw_impl(sf::RenderWindow& window, PlayLevelCoreContextData& context)
{
    window.clear();

    auto const player_path = context.player.animation_controller.texture_key;

    sf::View view = window.getDefaultView();

    auto const& texture_container_player = AssetManager::instance().get_animated_texture_container(player_texture_path);

    auto sprite = texture_container_player.get_as_sprite(CreatureSequence::casting_swing, context.player.animation_controller.current_frame,
        {context.player.position.x, context.player.position.y});

    context.scene.draw(window);

    //sprite.setScale(3, 3);

    window.setView(view);

    window.draw(sprite);

#ifndef NDEBUG
    draw_debug_info(window, view, context.player, context.fps);
#endif

    window.display();
}

} // namespace jeagle