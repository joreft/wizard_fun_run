cmake_minimum_required(VERSION 3.12)

project(dreams_are_real)

# Exporting the assets
file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})

find_program(CCACHE ccache)
if(CCACHE)
    set(CMAKE_CXX_COMPILER_LAUNCHER ${CCACHE})
endif()

set(CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})

set(SOURCES_DREAMS src/asset_manager.cpp
                   src/core.cpp
                   src/json11_from_file.cpp
                   src/play_level_core.cpp
                   src/play_level_draw.cpp
                   src/play_level_handle_input.cpp
                   src/play_level_update.cpp)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR} ${CMAKE_MODULE_PATH})

set (SOURCES_DREAMS_TESTS  ${SOURCES_DREAMS} tests/tests_main.cpp)

add_executable(dreams src/main.cpp ${SOURCES_DREAMS} src/menu.cpp)
add_executable(dreams_tests ${SOURCES_DREAMS_TESTS})

target_include_directories(dreams PUBLIC inc)
target_include_directories(dreams_tests PUBLIC inc)


target_compile_options(dreams PRIVATE -Wall -Wextra -pedantic -Wnon-virtual-dtor)
target_compile_features(dreams PRIVATE cxx_std_17)

target_compile_options(dreams_tests PRIVATE -Wall -Wextra -pedantic -Wnon-virtual-dtor)
target_compile_features(dreams_tests PRIVATE cxx_std_17)

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

# TODO verify whether imgui requires these
set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL)
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

set(DREAMS_DEPENDENCIES imgui
                        json11
                        Threads::Threads
                        Boost::Boost
                        spdlog::spdlog
                        fmt::fmt
                        sfml::sfml)

target_link_libraries(dreams PRIVATE ${DREAMS_DEPENDENCIES})
target_link_libraries(dreams_tests PRIVATE ${DREAMS_DEPENDENCIES} Catch2::Catch2)