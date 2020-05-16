#include "play_level_core.h"

#include "asset_manager.h"
#include "conversion.h"
#include "fonts.h"
#include "scene.h"

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
        case Player::State::walking:
            return "walking";
    }

    return "unknown";
}

#ifndef NDEBUG
static void draw_debug_info(sf::RenderWindow& window, sf::View view, Player const& player, int fps)
{
    std::vector<std::string> const debug_info
    {
        fmt::format("FPS: {}", fps),
        fmt::format("Position, x: {:.2f}, y: {:.2f}", player.position.x, player.position.y),
        fmt::format("Speed, x: {:.2f}, y: {:.2f}", player.speed.x, player.speed.y),
        fmt::format("Player state: {}", stringify_state(player.state))
        //fmt::format("Acceleration, x: {:.2f}, y: {:.2f}", player_newtonian->acceleration.x, player_newtonian->acceleration.y)
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

    auto const& texture_container_player = AssetManager::instance().get_animated_texture_container(paths::player_texture_name);

    auto sprite = texture_container_player.get_as_sprite(context.player.animation_controller.sequence, context.player.animation_controller.current_frame,
        {context.player.position.x, context.player.position.y});

    window.setView(view);
    context.scene.draw(window);

    sf::Texture const& ball_texture = AssetManager::instance().get_texture("assets/fireball.png");

    for (auto const& p : context.projectiles)
    {

        sf::Sprite ball(ball_texture);
        ball.setOrigin(14, 2);

        ball.setTextureRect(sf::IntRect{20 * p.frame, 0, 20, 10});
        ball.setPosition(p.collision.upper_left.x, p.collision.upper_left.y);

        window.draw(ball);
    }

    window.draw(sprite);

#ifndef NDEBUG
    draw_debug_info(window, view, context.player, context.fps);
#endif

    window.display();
}

} // namespace jeagle