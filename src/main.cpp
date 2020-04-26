#include "core.h"
#include "log.h"

#include <SFML/Window.hpp>
#include <spdlog/spdlog.h>

int main()
{
#ifndef NDEBUG
    // Global level for debug builds
    spdlog::set_level(spdlog::level::debug);
#endif

    LOG_INFO("Starting up codename dreams_are_real");

    //sf::VideoMode mode {500, 400, 32};
    auto const settings  = jeagle::WindowSettings{sf::VideoMode::getDesktopMode(),
                                                  std::string("Dreams are real"),
                                                  sf::Style::Fullscreen
                                                 };

    jeagle::start_main_menu();
    jeagle::core::run(settings);

    LOG_INFO("Exiting dreams_are_real");
    return 0;
}
