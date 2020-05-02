#include "core.h"
#include "fonts.h"
#include "log.h"

#include <SFML/Window.hpp>
#include <spdlog/spdlog.h>

int main()
{
#ifndef NDEBUG
    // Global level for debug builds
    spdlog::set_level(spdlog::level::debug);
#endif

    jeagle::fonts::global_preload_all_fonts();

    LOG_INFO("Starting up codename wizard_fun_run");

    //sf::VideoMode mode {500, 400, 32};
    auto const settings  = jeagle::WindowSettings{sf::VideoMode::getDesktopMode(),
                                                  std::string("Wizard Fun Run"),
                                                  sf::Style::Fullscreen
                                                 };

    jeagle::start_main_menu();
    jeagle::core::run(settings);

    LOG_INFO("Exiting wizard_fun_run");
    return 0;
}
