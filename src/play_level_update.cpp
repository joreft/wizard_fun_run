#include "physics.h"
#include "play_level_core.h"

namespace jeagle
{

static constexpr float max_player_walk_speed = 90;
static constexpr auto player_acceleration = Vector2f {250.f, 0.f}; // pixels per second

static constexpr void process_input_state(Player& player, KeyInputState const& input)
{
    if (input.left_pressed)
    {
        player.input_state.direction = Player::InputState::MoveDirection::left;
    }
    else if (input.right_pressed)
    {
        player.input_state.direction = Player::InputState::MoveDirection::right;
    }
    else
    {
        player.input_state.direction = Player::InputState::MoveDirection::none;
    }
}

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
        transition_player_state(Player::State::casting_swing, player);
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
        auto const direction = player.input_state.direction == Player::InputState::MoveDirection::right ? 1.f : -1.f;
        auto const maybe_new_speed = player.speed + player_acceleration * s_elapsed * direction;

        player.facing_left = direction == -1;

        if ((maybe_new_speed.x < max_player_walk_speed) && (maybe_new_speed.x > -max_player_walk_speed))
        {
            player.speed = maybe_new_speed;
        }

        constexpr float time_per_frame = 0.1f;
        if (player.state_accumulated_time > time_per_frame)
        {
            ++player.animation_controller.current_frame;
            player.animation_controller.current_frame = player.animation_controller.current_frame % 10;
            player.state_accumulated_time -= time_per_frame;
        }
    }
    else if (player.input_state.cast_requested)
    {
        player.speed = {0.f, 0.f};
        transition_player_state(Player::State::casting_swing, player);
    }
    else
    {
        transition_player_state(Player::State::idle, player);
    }
}

Projectile spawn_projectile(Player& player)
{
    Projectile pro{};

    pro.speed = {50, 0};
    pro.collision.upper_left.x = player.position.x + 29;
    pro.collision.upper_left.y = player.position.y + 10;
    pro.speed.x = 250;

    return pro;
}

void handle_player_casting_animation(Player& player, std::vector<Projectile>& projectiles, float s_elapsed)
{
    player.state_accumulated_time += s_elapsed;

    static bool did_cast = false;

    if (player.state_accumulated_time > 0.15f * 3.f && !did_cast)
    {
        projectiles.emplace_back(spawn_projectile(player));
        did_cast = true;
    }

    if (did_cast && player.state_accumulated_time > 0.15f * 5.f)
    {
        did_cast = false;
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

    process_input_state(context.player, context.key_input_state);

    if (context.player.state == Player::State::idle)
    {
        handle_player_idle_state(context.player, s_elapsed);
    }
    else if (context.player.state == Player::State::casting_swing)
    {
        handle_player_casting_animation(context.player, context.projectiles, s_elapsed);
    }
    else if (context.player.state == Player::State::walking)
    {
        handle_player_walking_state(context.player, s_elapsed);
    }

    for (auto& p : context.projectiles)
    {
        p.collision.upper_left.x += s_elapsed * p.speed.x;
        p.time_accumulator += s_elapsed;
        if (p.time_accumulator > 0.1f)
        {
            p.time_accumulator = 0;
            p.frame += 1;
            p.frame %= 7;
        }
    }

    context.player.position += s_elapsed * context.player.speed;


}

} // namespace jeagle