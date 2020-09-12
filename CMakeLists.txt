cmake_minimum_required(VERSION 3.12)

project(wizard_fun_run)

# Exporting the assets
file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})

find_program(CCACHE ccache)
if(CCACHE)
    set(CMAKE_CXX_COMPILER_LAUNCHER ${CCACHE})
endif()

set(CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})

set(SOURCES_WIZARD_FUN_RUN src/asset_manager.cpp
                   src/core.cpp
                   src/fonts.cpp
                   src/json11_from_file.cpp
                   src/level_editor.cpp
                   src/menu.cpp
                   src/physics.cpp
                   src/play_level_core.cpp
                   src/play_level_draw.cpp
                   src/play_level_handle_input.cpp
                   src/play_level_update.cpp
                   src/scene.cpp
                   src/texture_container.cpp)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR} ${CMAKE_MODULE_PATH})

set (SOURCES_WIZARD_FUN_RUN_TESTS  ${SOURCES_WIZARD_FUN_RUN} tests/tests_main.cpp)

add_executable(wizard_fun_run src/main.cpp ${SOURCES_WIZARD_FUN_RUN})
add_executable(wizard_fun_run_tests ${SOURCES_WIZARD_FUN_RUN_TESTS})

target_include_directories(wizard_fun_run PUBLIC inc)
target_include_directories(wizard_fun_run_tests PUBLIC inc)


target_compile_options(wizard_fun_run PRIVATE -Wall -Wextra -pedantic -Wnon-virtual-dtor)
target_compile_features(wizard_fun_run PRIVATE cxx_std_17)

target_compile_options(wizard_fun_run PRIVATE -Wall -Wextra -pedantic -Wnon-virtual-dtor)
target_compile_features(wizard_fun_run_tests PRIVATE cxx_std_17)

find_program(CONAN conan)
if (NOT CONAN)
    message(FATAL_ERROR "This project needs conan for dependency management: see https://conan.io")
endif()

if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
  message(
    STATUS
      "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
  file(
    DOWNLOAD
      "https://raw.githubusercontent.com/conan-io/cmake-conan/v0.12/conan.cmake"
      "${CMAKE_BINARY_DIR}/conan.cmake")
endif()

include(${CMAKE_BINARY_DIR}/conan.cmake)

conan_add_remote(NAME bincrafters URL https://api.bintray.com/conan/bincrafters/public-conan)

conan_cmake_run(CONANFILE
                conanfile.txt
                BASIC_SETUP
                CONAN_COMMAND
                ${CONAN_CMD}
                CMAKE_TARGETS
                BUILD
                missing)

find_package(sfml REQUIRED)
find_package(spdlog REQUIRED)
find_package(fmt REQUIRED)
find_package(Catch2 REQUIRED)
find_package(Boost REQUIRED)

add_subdirectory(external)
add_subdirectory(libs)

# TODO verify whether imgui requires these
set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL)
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

set(WIZARD_FUN_RUN_DEPENDENCIES imgui
                                json11
                                platformerutils
                                Threads::Threads
                                Boost::Boost
                                spdlog::spdlog
                                fmt::fmt
                                sfml::sfml)

target_link_libraries(wizard_fun_run PRIVATE ${WIZARD_FUN_RUN_DEPENDENCIES})
target_link_libraries(wizard_fun_run_tests PRIVATE ${WIZARD_FUN_RUN_DEPENDENCIES} Catch2::Catch2)
