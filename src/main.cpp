#include "log.h"

#include <spdlog/spdlog.h>

int main()
{
#ifndef NDEBUG
    // Global level for debug builds
    spdlog::set_level(spdlog::level::debug);
#endif

    LOG_INFO("Starting up codename dreams_are_real");

    LOG_INFO("Exiting dreams_are_real");
    return 0;
}
