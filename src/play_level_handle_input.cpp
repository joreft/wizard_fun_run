#include "play_level_core.h"

#include "core.h"

namespace jeagle
{

void play_level_core_handle_input_impl(sf::Event const &event, PlayLevelCoreContextData &context)
{
    auto& key_input_state = context.key_input_state;
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

        if (event.key.code == sf::Keyboard::Right)
        {
            key_input_state.right_pressed = true;
        }

        if (event.key.code == sf::Keyboard::Left)
        {
            key_input_state.left_pressed = true;
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

        if (event.key.code == sf::Keyboard::Right)
        {
            key_input_state.right_pressed = false;
        }

        if (event.key.code == sf::Keyboard::Left)
        {
            key_input_state.left_pressed = false;
        }
    }
}

} // namespace jeagle