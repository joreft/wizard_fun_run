#include "play_level_core.h"

#include "asset_manager.h"
#include "fonts.h"
#include "conversion.h"

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
    auto const text_size = 30;
    auto current_position = 0;
    sf::RectangleShape text_background;
    text_background.setPosition(view.getCenter().x - view.getSize().x / 2, view.getCenter().y - view.getSize().y / 2);
    text_background.setFillColor(sf::Color{255, 255, 255, 100});
    text_background.setSize({600, static_cast<float>(debug_info.size() * text_size + 20)});
    window.draw(text_background);
    for (auto const& str : debug_info)
    {
        sf::Text text;
        text.setString(str);
        text.setCharacterSize(text_size);
        text.setFont(AssetManager::instance().get_font(fonts::inconsolata));
        text.setPosition({10 + view.getCenter().x - view.getSize().x / 2, view.getCenter().y - view.getSize().y / 2 + current_position});
        text.setFillColor(sf::Color::Black);
        current_position += text_size;
        window.draw(text);
    }
}

void play_level_core_draw_impl(sf::RenderWindow& window, PlayLevelCoreContextData& context)
{
    window.clear();

    auto const spritesheet_pos = context.player.animation_controller.frames_in_state.at(context.player.animation_controller.current_frame);

    auto const player_path = context.player.animation_controller.texture_key;

    sf::View view{};
    AssetManager::instance().ensure_texture_loaded(player_path);
    sf::Sprite player_sprite(AssetManager::instance().get_texture(player_path));

    player_sprite.setTextureRect(int_rect_from_box(spritesheet_pos));
    player_sprite.setPosition(context.player.position.x, context.player.position.y);

#ifndef NDEBUG
    draw_debug_info(window, view, context.player, context.fps);
#endif

    window.draw(player_sprite);

    window.display();
}

} // namespace jeagle