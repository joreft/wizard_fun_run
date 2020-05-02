#include "play_level_core.h"

namespace jeagle
{

constexpr void handle_player_idle_state(Player& player)
{
    if (player.input_state.cast_requested)
    {
        // initiate  casting state
        player.state = Player::State::casting_swing;
        player.state_accumulated_time = 0;
        player.animation_controller.frames_in_state = player.asset_data.casting_boxes;
        player.animation_controller.millisecond_per_frame = 200;
        player.animation_controller.current_frame = 0;

        player.input_state.cast_requested = false;
    }
    else if (player.input_state.direction != Player::InputState::MoveDirection::none)
    {


    }
    else
    {

    }

}

constexpr void handle_player_casting_animation(Player& player, float s_elapsed)
{
    player.state_accumulated_time += s_elapsed;

    if (player.state_accumulated_time > 0.15f * 4.f)
    {
        player.state_accumulated_time = 0;
        player.state = Player::State::idle;
        player.animation_controller.current_frame = 0;
    }
    else
    {
        player.animation_controller.current_frame = (float)player.state_accumulated_time / 0.15f;
    }
}

void play_level_core_update_impl(PlayLevelCoreContextData& context, float s_elapsed)
{
    context.fps = static_cast<int>(1.f / s_elapsed);

    if (context.player.state == Player::State::idle)
    {
        handle_player_idle_state(context.player);
    }
    else if (context.player.state == Player::State::casting_swing)
    {
        handle_player_casting_animation(context.player, s_elapsed);
    }

    context.player.position += s_elapsed * context.player.speed;


}

} // namespace jeagle