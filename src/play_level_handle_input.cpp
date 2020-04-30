#include "play_level_core.h"

#include "core.h"

namespace jeagle
{

void play_level_core_handle_input_impl(sf::Event const &event, PlayLevelCoreContextData &context)
{
    if (event.type == sf::Event::KeyPressed)
    {
        if (event.key.code == sf::Keyboard::Escape)
        {
            core::set_should_pop_state();
        }

        if (event.key.code == sf::Keyboard::Z)
        {
            context.player.input_state.cast_requested = true;
        }

        if (event.key.code == sf::Keyboard::Down)
        {
            context.player.input_state.direction = Player::InputState::MoveDirection::down;
        }

        if (event.key.code == sf::Keyboard::Right)
        {
            context.player.input_state.direction = Player::InputState::MoveDirection::right;
        }

        if (event.key.code == sf::Keyboard::Down)
        {
            context.player.input_state.direction = Player::InputState::MoveDirection::down;
        }

        if (event.key.code == sf::Keyboard::Right)
        {
            context.player.input_state.direction = Player::InputState::MoveDirection::right;
        }
        if (event.key.code == sf::Keyboard::Up)
        {
            context.player.input_state.direction = Player::InputState::MoveDirection::up;
        }

        if (event.key.code == sf::Keyboard::Left)
        {
            context.player.input_state.direction = Player::InputState::MoveDirection::left;
        }

    }

    if (event.type == sf::Event::KeyReleased)
    {
        if (event.key.code == sf::Keyboard::Escape)
        {
            core::set_should_pop_state();
        }

        if (event.key.code == sf::Keyboard::Z)
        {
            context.player.input_state.cast_requested = false;
        }

        if (event.key.code == sf::Keyboard::Down)
        {
            //context.player.state = Player::State::CASTING_SWING;
        }

        if (event.key.code == sf::Keyboard::Right)
        {
            // context.player.state = Player::State::;
        }
    }

    }

} // namespace jeagle