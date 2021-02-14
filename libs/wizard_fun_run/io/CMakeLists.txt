add_library(platformerutils src/rectangle_collision.cpp)
target_compile_features(platformerutils PRIVATE cxx_std_17)
target_include_directories(platformerutils PUBLIC inc)
target_link_libraries(platformerutils json11 fmt::fmt)
