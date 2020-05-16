#include "asset_manager.h"
#include "box.h"
#include "core.h"
#include "log.h"
#include "play_level_core.h"

#include <algorithm>
#include <functional>
#include <vector>

namespace jeagle
{

struct MenuEntry
{
    std::string const text;
    sf::Keyboard::Key const hotkey;
    std::function<void(void)> const on_click;

    struct RuntimeState
    {
        bool is_hovered = false;
        Box<int> box {};
    } runtime_state {};
};

struct MenuContextData
{
    std::vector<MenuEntry> menu_entries;
};

namespace
{

auto constexpr scale = 640.f/1920.f; // TODO fix the sprites

void init_menu_impl(sf::RenderWindow& window, MenuContextData& menu_context)
{
    constexpr int width_pixels = 590.f * scale;
    constexpr int height_pixels = 70.f * scale;
    constexpr int spacing_pixels = 5.f * scale;

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

void handle_menu_input_impl(sf::Event const& event, MenuContextData& menu_context, sf::RenderWindow& window)
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

    auto const mouse_position = window.mapPixelToCoords(sf::Mouse::getPosition(window));
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

constexpr char const background[] =   "assets/menu_background.png";
constexpr char const menu_texture[] = "assets/menu_entry.png";

void draw_menu_impl(sf::RenderWindow& window, MenuContextData const& menu_context)
{
    AssetManager::instance().ensure_texture_loaded(background);
    AssetManager::instance().ensure_texture_loaded(menu_texture);

    sf::Sprite background_sprite(AssetManager::instance().get_texture(background));

    window.clear();

    background_sprite.setScale(scale, scale);
    window.draw(background_sprite);

    for (auto const& entry : menu_context.menu_entries)
    {
        sf::Sprite menu_sprite(AssetManager::instance().get_texture(menu_texture));
        auto const x = entry.runtime_state.box.upper_left.x;
        auto const y = entry.runtime_state.box.upper_left.y;

        sf::Text text;
        text.setFont(AssetManager::instance().get_default_font());
        text.setCharacterSize(42);
        text.setFillColor(sf::Color::Black); text.setOutlineColor(sf::Color::Black);
        text.setString(entry.text);
        text.setScale(scale, scale);

        auto const text_pos_x = (entry.runtime_state.box.size.x - scale * text.getLocalBounds().width) / 2 + entry.runtime_state.box.upper_left.x;
        text.setPosition(text_pos_x, static_cast<float>(entry.runtime_state.box.upper_left.y) + 3.f);

        if (entry.runtime_state.is_hovered)
        {
            menu_sprite.setTextureRect(sf::IntRect{0, 70, 590, 70});
        }
        else
        {
            menu_sprite.setTextureRect(sf::IntRect{0, 0, 590, 70});
        }

        menu_sprite.setScale(scale, scale);

        menu_sprite.setPosition(x, y);
        window.draw(menu_sprite);
        window.draw(text);
    }

    window.setView(window.getDefaultView());

    window.display();
}

} // namespace anonymous

struct Menu : public State
{
    explicit Menu(MenuContextData context)
     : context(std::move(context))
    {}

    void init() override
    {
        init_menu_impl(*m_window, context);
    }

    void handle_input(sf::Event const& event) override
    {
        handle_menu_input_impl(event, context, *m_window);
    }

    void update(float) override
    {}

    void draw() override
    {
        draw_menu_impl(*m_window, context);
    }

private:
    MenuContextData context {};
};

void start_main_menu()
{
    std::vector<MenuEntry> main_menu_entries
    {
          MenuEntry {"Play [p]", sf::Keyboard::Key::P, start_play_level_core}
        , MenuEntry {"Create [c]", sf::Keyboard::Key::C, start_level_editor}
        , MenuEntry {"Exit [Esc]", sf::Keyboard::Key::Escape, core::set_should_pop_state}
    };

    std::unique_ptr<State> menu = std::make_unique<Menu>(MenuContextData{std::move(main_menu_entries)});
    core::push_state(std::move(menu));
}

} // namepsace jeagle