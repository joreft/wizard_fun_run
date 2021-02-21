cmake_minimum_required(VERSION 3.12)

project(wizard_fun_run)

# Exporting the assets
file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})

find_program(CCACHE ccache)
if(CCACHE)
    set(CMAKE_CXX_COMPILER_LAUNCHER ${CCACHE})
endif()

set(CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR} ${CMAKE_MODULE_PATH})

add_executable(game_box src/main.cpp)

target_compile_features(game_box PRIVATE cxx_std_17)

if(MSVC)
    target_compile_options(game_box INTERFACE /W4 /WX "/permissive-")
else()
    target_compile_options(game_box PRIVATE -Wall -Wextra -pedantic -Wnon-virtual-dtor)
endif()

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
      "https://raw.githubusercontent.com/conan-io/cmake-conan/v0.15/conan.cmake"
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
find_package(catch2 REQUIRED)
find_package(Boost REQUIRED)

add_subdirectory(libs)

set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)

add_subdirectory(external)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

set(WIZARD_FUN_RUN_DEPENDENCIES wizard_fun_run
                                gameloop
                                log
                                Threads::Threads
                                Boost::Boost
                                spdlog::spdlog
                                fmt::fmt
                                sfml::sfml)

target_link_libraries(game_box PRIVATE ${WIZARD_FUN_RUN_DEPENDENCIES})
