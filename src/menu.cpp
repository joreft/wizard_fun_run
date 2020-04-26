#include "asset_manager.h"
#include "box.h"
#include "core.h"
#include "log.h"

namespace jeagle
{

struct MenuEntry
{
    std::string const text;
    sf::Keyboard::Key const hotkey;
    std::function<void(void)> const on_click;

    struct State
    {
        bool is_hovered = false;
        Box<int> box {};
    } runtime_state {};
};

struct MenuContextData
{
    std::optional<int> selected_menu_index;

    std::array<MenuEntry, 3> menu_entries
    {
          MenuEntry {"Play [p]", sf::Keyboard::Key::P, core::set_should_pop_state}
        , MenuEntry {"Create [c]", sf::Keyboard::Key::C, core::set_should_pop_state}
        , MenuEntry {"Exit [Esc]", sf::Keyboard::Key::Escape, core::set_should_pop_state}
    };
};

namespace
{

void init_menu_impl(sf::RenderWindow& window, MenuContextData& menu_context)
{
    constexpr int width_pixels = 590;
    constexpr int height_pixels = 70;
    constexpr int spacing_pixels = 5;

    auto const size_x = static_cast<int>(window.getSize().x);
    auto const size_y = static_cast<int>(window.getSize().y);

    auto const offset_x = (size_x - width_pixels) / 2;

    int running_offset_y = size_y / 4;
    for (auto& entry : menu_context.menu_entries)
    {
        entry.runtime_state.box.upper_left = Vector2i {offset_x, running_offset_y};
        entry.runtime_state.box.size = Vector2i {width_pixels, height_pixels};
        running_offset_y += height_pixels + spacing_pixels;
    }
}

void handle_menu_input_impl(sf::Event const& event, MenuContextData& menu_context)
{
    auto const& menu_entries = menu_context.menu_entries;

    bool mouse_clicked = false;

    if (event.type == sf::Event::KeyPressed)
    {
        auto is_key = [key = event.key.code](auto const& entry)
        {
            return entry.hotkey == key;
        };

        auto const entry_at = std::find_if(menu_entries.cbegin(), menu_entries.cend(), is_key);
        if (entry_at != menu_entries.cend())
        {
            entry_at->on_click();
        }
    }

    if (event.type == sf::Event::MouseButtonPressed)
    {
        if (event.mouseButton.button == sf::Mouse::Button::Left)
        {
            mouse_clicked = true;
        }
    }

    auto const mouse_position = sf::Mouse::getPosition();
    auto const mouse_as_point = Vector2i{mouse_position.x, mouse_position.y};

    for (auto& entry : menu_context.menu_entries)
    {
        if (point_inside_box(mouse_as_point, entry.runtime_state.box))
        {
            entry.runtime_state.is_hovered = true;

            if (mouse_clicked)
            {
                entry.on_click();
            }
        }
        else
        {
            entry.runtime_state.is_hovered = false;
        }

    }

}

std::string const background =   "assets/menu_background.png";
std::string const menu_texture = "assets/menu_entry.png";

void draw_menu_impl(sf::RenderWindow& window, MenuContextData const& menu_context)
{
    AssetManager::instance().ensure_loaded(background);
    AssetManager::instance().ensure_loaded(menu_texture);

    sf::Sprite background_sprite(AssetManager::instance().get(background));

    window.clear();
    window.draw(background_sprite);

    for (auto const& entry : menu_context.menu_entries)
    {
        sf::Sprite menu_sprite(AssetManager::instance().get(menu_texture));
        auto const x = entry.runtime_state.box.upper_left.x;
        auto const y = entry.runtime_state.box.upper_left.y;

        sf::Text text;
        text.setFont(AssetManager::instance().getDefaultFont());
        text.setCharacterSize(50);
        text.setFillColor(sf::Color::Black); text.setOutlineColor(sf::Color::Black);
        text.setString(entry.text);

        auto const text_pos_x = (entry.runtime_state.box.size.x - text.getLocalBounds().width) / 2 + entry.runtime_state.box.upper_left.x;
        text.setPosition(text_pos_x, entry.runtime_state.box.upper_left.y + 2);

        if (entry.runtime_state.is_hovered)
        {
            menu_sprite.setTextureRect(sf::IntRect{0, 70, 590, 70});
        }
        else
        {
            menu_sprite.setTextureRect(sf::IntRect{0, 0, 590, 70});
        }

        menu_sprite.setPosition(x, y);
        window.draw(menu_sprite);
        window.draw(text);
    }

    window.display();
}

} // namespace anonymous

struct Menu : public State
{
    MenuContextData context;

    void init() override
    {
        init_menu_impl(*m_window, context);
    }

    void handle_input(sf::Event const& event) override
    {
        handle_menu_input_impl(event, context);
    }

    void update(float) override
    {}

    void draw() override
    {
        draw_menu_impl(*m_window, context);
    }
};

void start_menu()
{
    std::unique_ptr<State> menu = std::make_unique<Menu>();
    core::push_state(std::move(menu));
}

} // namepsace jeagle