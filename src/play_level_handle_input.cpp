#include "play_level_core.h"

#include "core.h"

namespace jeagle
{

void play_level_core_handle_input_impl(sf::Event const &event, PlayLevelCoreContextData &context, KeyInputState& key_input_state)
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

    if (key_input_state.left_pressed)
    {
        context.player.input_state.direction = Player::InputState::MoveDirection::left;
    }
    else if (key_input_state.right_pressed)
    {
        context.player.input_state.direction = Player::InputState::MoveDirection::right;
    }
    else
    {
        context.player.input_state.direction = Player::InputState::MoveDirection::none;
    }

    if (key_input_state.jump_pressed)
    {

    }

    }

} // namespace jeagle