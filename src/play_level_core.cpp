#include "play_level_core.h"

#include "asset_manager.h"
#include "core.h"
#include "texture_paths.h"

namespace jeagle
{

struct PlayLevelCore : public State
{
  explicit PlayLevelCore(PlayLevelCoreContextData context)
      : context(std::move(context))
  {}

  void init() override
  {
      auto& player = context.player;
      player.asset_data = load_player_asset_data();
      player.animation_controller.texture_key = "assets/character.png";
      player.animation_controller.millisecond_per_frame = 200;
      player.animation_controller.current_frame = 0;

      MovableBody player_body_initialised {};

      player_body_initialised.collision_box =  {100, 360/2, 32, 48};

      auto* player_body = context.physics_world.register_movable_body(player_body_initialised);
      player.physics_handle = player_body; //241
      player.physics_handle->collision_box.upper_left.y = 241 - player.asset_data.casting_boxes.at(0).size.y;

      AssetManager::instance().ensure_animated_texture_loaded(paths::player_texture_name);
      AssetManager::instance().ensure_texture_loaded("assets/fireball.png");
  }

  void handle_input(sf::Event const& event) override
  {
      play_level_core_handle_input_impl(event, context);
  }

  void update(float s_elapsed) override
  {
      play_level_core_update_impl(context, s_elapsed);
  }

  void draw() override
  {
      play_level_core_draw_impl(*m_window, context);
  }

private:
    PlayLevelCoreContextData context {};
};

void start_play_level_core()
{
    std::unique_ptr<State> level = std::make_unique<PlayLevelCore>(PlayLevelCoreContextData{});
    core::push_state(std::move(level));
}

} // namespace jeagle