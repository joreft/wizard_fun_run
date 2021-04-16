#include "core.h"
#include "log.h"
#include "wizard_fun_run.h"

#include <SFML/Window.hpp>
#include <spdlog/spdlog.h>

int main()
{
#ifndef NDEBUG
    // Global level for debug builds
    spdlog::set_level(spdlog::level::debug);
#endif

    LOG_INFO("Wizard Fun Run: Searching for the pie in the sky");

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
