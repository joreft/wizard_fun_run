add_library(wizard_fun_run_sfml src/asset_manager.cpp
            src/fonts.cpp
            src/level_editor.cpp
            src/menu.cpp
            src/play_level_core.cpp
            src/play_level_draw.cpp
            src/play_level_handle_input.cpp
            src/play_level_update.cpp
            src/scene.cpp
            src/texture_container.cpp)

target_compile_features(wizard_fun_run_sfml PRIVATE cxx_std_17)
target_include_directories(wizard_fun_run_sfml PUBLIC inc)
target_link_libraries(wizard_fun_run_sfml PRIVATE sfml::sfml imgui gameloop Boost::Boost log platformerutils utils wizard_fun_run log)