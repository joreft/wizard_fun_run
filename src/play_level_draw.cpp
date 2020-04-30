#include "play_level_core.h"

#include "asset_manager.h"

namespace jeagle
{

static sf::IntRect int_rect_from_box(Box<int> const& box)
{
    return {box.upper_left.x, box.upper_left.y, box.size.x, box.size.y};
}

void play_level_core_draw_impl(sf::RenderWindow& window, PlayLevelCoreContextData& context)
{
    window.clear();

    auto const spritesheet_pos = context.player.animation_controller.frames_in_state.at(context.player.animation_controller.current_frame);

    auto const player_path = context.player.animation_controller.texture_key;

    AssetManager::instance().ensure_texture_loaded(player_path);
    sf::Sprite player_sprite(AssetManager::instance().get_texture(player_path));

    player_sprite.setTextureRect(int_rect_from_box(spritesheet_pos));
    player_sprite.setPosition(context.player.position.x, context.player.position.y);

    window.draw(player_sprite);

    window.display();
}

} // namespace jeagle