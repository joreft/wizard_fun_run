add_library(wizard_fun_run  src/asset_manager.cpp
                            src/fonts.cpp
                            src/level_editor.cpp
                            src/menu.cpp
                            src/physics.cpp
                            src/play_level_core.cpp
                            src/play_level_draw.cpp
                            src/play_level_handle_input.cpp
                            src/play_level_update.cpp
                            src/scene.cpp
                            src/texture_container.cpp)

target_compile_features(wizard_fun_run PRIVATE cxx_std_17)
target_include_directories(wizard_fun_run PUBLIC exported_inc PRIVATE inc)
target_link_libraries(wizard_fun_run PRIVATE sfml::sfml imgui gameloop Boost::Boost log platformerutils utils log)