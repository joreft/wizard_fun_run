#include "core.h"

//#include "level_editor.h"

#include "log.h"

#include "asset_manager.h"
#include "core.h"
#include "scene.h"

#include <json11/json11.hpp>

#include <imgui.h>
#include <imgui-SFML.h>

#include <filesystem>
#include <fstream>
#include <sstream>
#include <memory>

namespace jeagle
{

static constexpr std::string_view spritesheet_config("assets/spritesheet_config.json");

static constexpr const int grid_size_pixels = 16;

struct Spritesheet
{
    std::string friendly_name;
    Vector2i tile_size;
    sf::Texture const& texture;
    std::string texture_path;

    static Spritesheet from(json11::Json const& json)
    {
        auto const spritesheet_path = json["path"].string_value();
        AssetManager::instance().ensure_texture_loaded(spritesheet_path);

        return {json["friendly_name"].string_value(),
                Vector2i{json["dimensions"]["x"].int_value(),
                         json["dimensions"]["y"].int_value()},

                AssetManager::instance().get_texture(spritesheet_path),
                spritesheet_path};
    }
};

std::vector<Spritesheet> spritesheets_from_config_file()
{
    std::ifstream sprites_cfg(std::string(spritesheet_config).c_str());
    std::stringstream buffer;
    buffer << sprites_cfg.rdbuf();

    std::string e;
    auto const spritesheet_json = json11::Json::parse(buffer.str(), e);

    if (!e.empty())
    {
        LOG_DEBUG("Parsing json from file {}, failed with {}", spritesheet_config, e);
    }

    std::vector<Spritesheet> out;
    for (auto const& sheet : spritesheet_json["spritesheets"].array_items())
    {
        out.push_back(Spritesheet::from(sheet));
    }
    return out;
}

struct Selected : public sf::Drawable
{
    Selected(std::string texture_path, Box<int> position_in_texture)
        : texture_path(std::move(texture_path)), position_in_texture(std::move(position_in_texture))
    {}

    sf::VertexArray vertex_array;
    std::string texture_path;
    Box<int> position_in_texture;

    Box<int> current_position;

    void set_box_position_and_size(Box<int> box, bool snap_to_grid_mode = false)
    {
        if (snap_to_grid_mode)
        {
            box.upper_left.x = (static_cast<int>(box.upper_left.x) / grid_size_pixels) * grid_size_pixels + 1;
            box.upper_left.y = (static_cast<int>(box.upper_left.y) / grid_size_pixels) * grid_size_pixels + 1;
        }
        current_position = box;
        vertex_array = sf::VertexArray(sf::Quads, 0);
        add_to_vertex_array_quads(position_in_texture, box, vertex_array);
    }
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override
    {
        states.texture = &AssetManager::instance().get_texture(texture_path);
        target.draw(vertex_array, states);
    }
};

class LevelEditor : public State
{
public:
    LevelEditor()
    {
        ImGui::CreateContext();
        spritesheets = spritesheets_from_config_file();
    }

    ~LevelEditor()
    {
        ImGui::DestroyContext();
    }

    void init()
    {
        ImGui::SFML::Init(*m_window);
        scene_view.setSize(640, 360);
        scene_view.setCenter(640/2, 360/2);
    }

    void set_window(sf::RenderWindow *new_window) override
    {
        m_window = new_window;
    }

    void handle_input(sf::Event const& event) override
    {
        ImGui::SFML::ProcessEvent(event);

        if (event.type == sf::Event::KeyPressed)
        {
            if (event.key.code == sf::Keyboard::Escape)
            {
                core::set_should_pop_state();
            }
            if (event.key.code == sf::Keyboard::C)
            {
                selected = std::nullopt;
            }
            if (event.key.code == sf::Keyboard::S)
            {
                if (current_scene)
                {
                    current_scene->save();
                }
            }

            if (event.key.code == sf::Keyboard::Escape)
            {
                core::set_should_pop_state();
            }
        }
        static bool middle_mouse_pressed = false;

        if (event.type == sf::Event::MouseButtonPressed)
        {
            if (event.mouseButton.button == sf::Mouse::Left)
            {
                if (selected && current_scene)
                {
                    auto const selected_collision_box = non_colliding_block ? std::nullopt :
                                                        std::optional<Box<int>>(selected->current_position);


                    current_scene->add_tile(selected->current_position, selected_collision_box, selected->texture_path, selected->position_in_texture);
                }
            }
            if (event.mouseButton.button == sf::Mouse::Right)
            {
                selected = std::nullopt;
            }

            if (event.mouseButton.button == sf::Mouse::Middle)
            {
                middle_mouse_pressed = true;
            }
        }

        if (event.type == sf::Event::MouseButtonReleased)
        {
            if (event.mouseButton.button == sf::Mouse::Middle)
            {
                middle_mouse_pressed = false;
            }
        }

        if (middle_mouse_pressed && event.type == sf::Event::MouseMoved)
        {
            auto const moved = sf::Mouse::getPosition() - last_frame_mouse_position;
            scene_view.move({static_cast<float>(-moved.x), static_cast<float>(-moved.y)});
        }


        static constexpr int default_width {grid_size_pixels};
        auto const mouse_pos =
            m_window->mapPixelToCoords(sf::Mouse::getPosition(*m_window));

        if (selected)
        {
            selected->set_box_position_and_size(Box<int>{{static_cast<int>(mouse_pos.x), static_cast<int>(mouse_pos.y)},
                                                    {default_width, default_width * (selected->position_in_texture.size.y)/selected->position_in_texture.size.x}}, snap_to_grid_mode);
        }

        last_frame_mouse_position = sf::Mouse::getPosition();
    }

    void update(float s_elapsed) override
    {
        auto const delta_time = sf::seconds(s_elapsed);
        ImGui::SFML::Update(*m_window, delta_time);
        ImGui::Begin("Base edit panel");

        if (ImGui::Button("Select scene"))
        {
            ImGui::OpenPopup("my_file_popup");
        }

        if (ImGui::Button("Delete level content"))
        {
            if (current_scene)
            {
                current_scene->erase();
            }
        }

        ImGui::Checkbox("Snap to grid mode", &snap_to_grid_mode);
        ImGui::Checkbox("No collision box mode", &non_colliding_block);

        if (ImGui::BeginPopup("my_file_popup"))
        {
            setup_file_menu();
            ImGui::EndPopup();
        }
        create_buttons_for_each_spritesheet();

        if (selected_spritesheet)
        {
            create_button_grid_from_spritesheet(*selected_spritesheet);
        }

        // Use this at some point to do tile manipulation
        ImGui::Begin("Selected tile");
        ImGui::Button("Delete tile");
        ImGui::End();
        // end tile manipulation

        ImGui::End();
    }

    void draw() override
    {
        m_window->clear();
        if (current_scene)
        {
            current_scene->draw(*m_window);
        }

        if (selected)
        {
            m_window->draw(*selected);
        }

        m_window->setView(scene_view);

        ImGui::SFML::Render(*m_window);
        m_window->display();

    }

    void create_buttons_for_each_spritesheet()
    {
        for (auto& sheet : spritesheets)
        {
            if (ImGui::Button(sheet.friendly_name.c_str()))
            {
                LOG_DEBUG("Button pressed for {}", sheet.friendly_name);
                selected_spritesheet = &sheet;
            }
            ImGui::SameLine();
        }
        ImGui::NewLine();
    }

    void create_button_grid_from_spritesheet(Spritesheet const& spritesheet)
    {
        sprites = std::vector<sf::Sprite>();
        auto const size = spritesheet.texture.getSize();

        auto const x_max = static_cast<int>(size.x) / spritesheet.tile_size.x;
        auto const y_max = static_cast<int>(size.y) / spritesheet.tile_size.y;

        for (int y = 0; y < y_max; ++y)
        {
            for (int x = 0; x < x_max; ++x)
            {
                sf::Sprite sprite;
                sprite.setTexture(spritesheet.texture);
                sprite.setTextureRect(sf::IntRect(sf::Vector2i(0 + spritesheet.tile_size.x*x, 0 + spritesheet.tile_size.y*y),
                                                  sf::Vector2i(spritesheet.tile_size.x, spritesheet.tile_size.y)));
                sprites.push_back(sprite);
                auto const& elem = *(sprites.end() - 1);
                ImGui::PushID(y*spritesheet.tile_size.y + x);
                if (ImGui::ImageButton(elem))
                {
                    LOG_DEBUG("Button pressed at {}, {}", x, y);
                    selected.emplace(Selected(
                        spritesheet.texture_path ,
                        Box<int>{
                            {spritesheet.tile_size.x * x, spritesheet.tile_size.y * y},
                            {spritesheet.tile_size.x, spritesheet.tile_size.y }
                        }));
                }
                ImGui::PopID();

                if (x != (x_max - 1))
                    ImGui::SameLine();
            }
        }
    }

    void setup_file_menu()
    {
        for(auto const& p: std::filesystem::directory_iterator("../assets/levels"))
        {
            auto const scene_path = p.path();
            auto const filename = scene_path.filename().string();
            if (ImGui::MenuItem(filename.c_str()))
            {
                if (current_scene_loaded == scene_path.string()) continue; // Scene was already loaded

                current_scene = std::make_unique<Scene>(scene_path.string());
            }
        }
    }

private:
    sf::RenderWindow *m_window = nullptr;

    std::vector<sf::Sprite> sprites;

    Spritesheet* selected_spritesheet = nullptr; // TODO pointer is potentially invalidated if spritesheets is reallocated
    std::vector<Spritesheet> spritesheets;

    std::string current_scene_loaded = "";

    std::unique_ptr<Scene> current_scene = nullptr;

    std::optional<Selected> selected;

    sf::View scene_view;

    bool snap_to_grid_mode = true;
    bool non_colliding_block = false;

    sf::Vector2i last_frame_mouse_position;
};

void start_level_editor()
{
    auto state = std::make_unique<LevelEditor>();
    core::push_state(std::move(state));
}

} // namespace jeagle