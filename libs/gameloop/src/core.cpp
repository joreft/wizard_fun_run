#include "core.h"

#include "log.h"

#include <SFML/Window.hpp>
#include <SFML/System.hpp>

#include <memory>
#include <stack>

namespace jeagle
{

namespace core
{

using StatePtr = std::unique_ptr<State>;

static std::stack<StatePtr> game_states;
static bool should_pop_state = false;
static StatePtr next_state = nullptr;

static void add_state(StatePtr new_state)
{
    game_states.push(std::move(new_state));
}

void set_should_pop_state()
{
    LOG_DEBUG("Popping off current state");
    should_pop_state = true;
}

static void game_loop(sf::RenderWindow &window, float s_elapsed, sf::Event &event)
{
    auto& current_state = *game_states.top();
    while (window.pollEvent(event))
    {
        current_state.handle_input(event);
    }

    current_state.update(s_elapsed);
    current_state.draw();

    if (next_state != nullptr)
    {
        add_state(std::move(next_state));
        LOG_DEBUG("Setting next state window");
        game_states.top()->set_window(window);
        game_states.top()->init();
        next_state = nullptr;
    }
    else if (should_pop_state)
    {
        game_states.pop();
        if (game_states.empty())
        {
            window.close();
        }

        should_pop_state = false;
    }
}

void push_state(StatePtr state)
{
    next_state = std::move(state);
}

int run(WindowSettings const &settings)
{
    if (!next_state)
    {
        LOG_DEBUG("Fatal application error: No initial state");
        return -1; // No state was queued for state machine initialisation
    }

    auto window = sf::RenderWindow(settings.mode, settings.title, settings.style);

    LOG_DEBUG("Setting up render window with dimensions {}x{}", window.getSize().x, window.getSize().y);

    window.setVerticalSyncEnabled(true);

    // Make sure repeated key events never happen
    // e.g. double escapes in a row...
    window.setKeyRepeatEnabled(false);

    // The first state of the game
    // Usually a menu or some such
    add_state(std::move(next_state));
    game_states.top()->set_window(window);
    game_states.top()->init();

    sf::Clock clock;

    LOG_DEBUG("Entering the core game loop");
    while (window.isOpen())
    {
        sf::Event event;
        sf::Time elapsed = clock.restart();
        game_loop(window, elapsed.asSeconds(), event);
    }

    return 0;
}

} // namespace core
} // namespace jeagle


