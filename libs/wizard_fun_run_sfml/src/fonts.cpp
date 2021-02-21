#include "fonts.h"

#include "asset_manager.h"

namespace jeagle
{

namespace fonts
{

void global_preload_all_fonts()
{
    auto &asset_manager = AssetManager::instance();
    asset_manager.ensure_font_loaded(inconsolata);
    asset_manager.ensure_font_loaded(sedgwick);
}

} // namespace fonts
} // namespace jeagle