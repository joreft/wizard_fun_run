add_library(gameloop src/core.cpp)
target_compile_features(gameloop PRIVATE cxx_std_17)
target_include_directories(gameloop PUBLIC inc)
target_link_libraries(gameloop PRIVATE sfml::sfml log)
