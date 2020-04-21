#pragma once

#include <SFML/Window.hpp>
#include <memory>
#include <SFML/Graphics.hpp>
#include <stdexcept>

namespace jeagle
{

struct CoreError : public std::runtime_error
{
    CoreError(std::string what) : std::runtime_error(std::move(what))
    {}
};

/**
 * There is sort of a contract here, that all deriving classes will
 * always have the SFML render window as a valid pointer
 *
 * i.e. you can safely assume that the window is not null
 */
class State
{
public:
    virtual ~State(){};
    virtual void init() = 0;

    virtual void handle_input(sf::Event const& event) = 0;
    virtual void update(float s_elapsed) = 0;
    virtual void draw() = 0;

    virtual void set_window(sf::RenderWindow *window)
    {
        if (!window)
        {
            throw CoreError("Missing render window when setting up a new state");
        }

        m_window = window;
    }

    sf::RenderWindow* m_window;
};

struct WindowSettings
{
    sf::VideoMode mode;
    std::string title;
    uint32_t style;
};

namespace core
{

void push_state(std::unique_ptr<State> state);
void set_should_pop_state();
int run(WindowSettings const &settings);

} // namespace core

// states
void start_menu();

} // namespace jeagle
