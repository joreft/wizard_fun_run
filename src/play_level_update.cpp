#include "play_level_core.h"

namespace jeagle
{

static constexpr float max_player_walk_speed = 120;
static constexpr auto player_acceleration = Vector2f {250.f, 0.f}; // pixels per second


constexpr void transition_player_state(Player::State const new_state, Player& player)
{
    player.state_accumulated_time = 0;
    player.state = new_state;

    // set up animation controller stuff
    player.animation_controller.current_frame = 0;
    player.animation_controller.millisecond_per_frame = 200;

    switch (new_state)
    {
    case Player::State::walking:
        player.animation_controller.sequence = CreatureSequence::walking;
        break;
    case Player::State::casting_swing:
        player.animation_controller.sequence = CreatureSequence::casting_swing;
        break;
    case Player::State::idle:
        player.animation_controller.sequence = CreatureSequence::casting_swing;
        break;
    }
}

constexpr void handle_player_idle_state(Player& player, float s_elapsed)
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
        if ((player.speed.x < max_player_walk_speed) && (player.speed.x > -max_player_walk_speed))
        {
            auto const direction = player.input_state.direction == Player::InputState::MoveDirection::right ? 1.f : -1.f;
            player.speed += player_acceleration * s_elapsed * direction;
            transition_player_state(Player::State::walking, player);
        }
    }
    else
    {
        player.speed = {0.f, 0.f};
    }
}

constexpr void handle_player_walking_state(Player& player, float s_elapsed)
{
    player.state_accumulated_time += s_elapsed;

    if (player.input_state.direction != Player::InputState::MoveDirection::none)
    {
        if ((player.speed.x < max_player_walk_speed) && (player.speed.x > -max_player_walk_speed))
        {
            auto const direction = player.input_state.direction == Player::InputState::MoveDirection::right ? 1.f : -1.f;
            player.speed += player_acceleration * s_elapsed * direction;
        }

        constexpr float time_per_frame = 0.1f;
        if (player.state_accumulated_time > time_per_frame)
        {
            player.animation_controller.current_frame++;
            player.animation_controller.current_frame = player.animation_controller.current_frame % 10;
            LOG_DEBUG("At frame {}", player.animation_controller.current_frame);
            player.state_accumulated_time -= time_per_frame;
        }
    }
    else if (player.input_state.cast_requested)
    {
        transition_player_state(Player::State::casting_swing, player);
    }
    else
    {
        transition_player_state(Player::State::idle, player);
    }
}

constexpr void handle_player_casting_animation(Player& player, float s_elapsed)
{
    player.state_accumulated_time += s_elapsed;

    if (player.state_accumulated_time > 0.15f * 5.f)
    {
        transition_player_state(Player::State::walking, player);
    }
    else
    {
        player.animation_controller.current_frame = (float)player.state_accumulated_time / 0.15f;
    }
}

constexpr void handle_player_walking(Player& player, float s_elapsed)
{
    player.state_accumulated_time += s_elapsed;

    if (player.state_accumulated_time > 0.15f * 5.f)
    {
        player.state_accumulated_time = 0;
        transition_player_state(Player::State::walking, player);
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
        handle_player_idle_state(context.player, s_elapsed);
    }
    else if (context.player.state == Player::State::casting_swing)
    {
        handle_player_casting_animation(context.player, s_elapsed);
    }
    else if (context.player.state == Player::State::walking)
    {
        handle_player_walking_state(context.player, s_elapsed);
    }

    context.player.position += s_elapsed * context.player.speed;


}

} // namespace jeagle