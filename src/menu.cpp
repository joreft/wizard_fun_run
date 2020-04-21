#include "asset_manager.h"
#include "box.h"
#include "core.h"
#include "log.h"

namespace jeagle
{

struct MenuEntry
{
    std::string text;
    sf::Keyboard::Key hotkey;
    std::function<void(void)> on_click;
};

std::array<MenuEntry, 1> const menu_entries
{
    {"Exit [Esc]", sf::Keyboard::Key::Escape, core::set_should_pop_state}
};

struct MenuContextData
{
};

namespace
{

void handle_input_impl(sf::Event const& event, MenuContextData& menu_context)
{
    if (event.type == sf::Event::KeyPressed)
    {
        if (event.key.code == sf::Keyboard::Escape)
        {
            LOG_DEBUG("Popping state");
            core::set_should_pop_state();
        }
    }
}

void update_impl(float s_elapsed, MenuContextData& menu_context)
{

}

std::string const background = "assets/menu_background.png";
void draw_impl(sf::RenderWindow& window, MenuContextData const& menu_context)
{
    AssetManager::instance().ensure_loaded(background);
    auto const texture = AssetManager::instance().get(background);
    sf::Sprite background_sprite(AssetManager::instance().get(background));
    window.clear();
    window.draw(background_sprite);

    window.display();
}

} // namespace anonymous

struct Menu : public State
{
    MenuContextData context;

    void init()
    {

    }

    void handle_input(sf::Event const& event)
    {
        handle_input_impl(event, context);
    }

    void update(float s_elapsed)
    {
        update_impl(s_elapsed, context);
    }

    void draw()
    {
        draw_impl(*m_window, context);
    }
};

void start_menu()
{
    std::unique_ptr<State> menu = std::make_unique<Menu>();
    core::push_state(std::move(menu));
}

} // namepsace jeagle