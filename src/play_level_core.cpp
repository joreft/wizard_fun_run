#include "play_level_core.h"

#include "core.h"

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
      player.animation_controller.frames_in_state = player.asset_data.casting_boxes;
      player.animation_controller.millisecond_per_frame = 200;
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