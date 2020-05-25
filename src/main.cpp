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

    auto const settings  = jeagle::WindowSettings{{640, 360},
                                                  std::string("Wizard Fun Run"),
                                                   sf::Style::Default
                                                 };

    jeagle::start_main_menu();

    // This runs the loop, nothing "graphical" happens until
    // here
    jeagle::core::run(settings);

    LOG_INFO("Exiting wizard_fun_run");
    return 0;
}
