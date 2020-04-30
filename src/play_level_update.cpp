#include "play_level_core.h"

namespace jeagle
{

void play_level_core_update_impl(PlayLevelCoreContextData& context, float s_elapsed)
{

    if (context.player.state == Player::State::idle)
    {
        if (context.player.input_state.cast_requested)
        {
            // initiate  casting state
            context.player.state = Player::State::casting_swing;
            context.player.state_accumulated_time = 0;
            context.player.animation_controller.frames_in_state = context.player.asset_data.casting_boxes;
            context.player.animation_controller.millisecond_per_frame = 200;
            context.player.animation_controller.current_frame = 0;

        }
        else if (context.player.input_state.direction != Player::InputState::MoveDirection::none)
        {


        }
        else
        {

        }
    }
    else if (context.player.state == Player::State::casting_swing)
    {
        context.player.state_accumulated_time += s_elapsed;
        if (context.player.state_accumulated_time > 0.15f * 4.f)
        {
            LOG_DEBUG("Accumulated more than 0.8s");
            context.player.state_accumulated_time = 0;
            context.player.state = Player::State::idle;
            context.player.animation_controller.current_frame = 0;
        }
        else
        {
            context.player.animation_controller.current_frame = (float)context.player.state_accumulated_time / 0.15f;
            LOG_DEBUG("Set frame to {}, with {}/{}", context.player.animation_controller.current_frame, context.player.state_accumulated_time, 0.15f);
        }
    }

    context.player.position += s_elapsed * context.player.speed;


}

} // namespace jeagle