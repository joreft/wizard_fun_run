

add_library(platformerutils platformerutils/src/rectangle_collision.cpp)
target_include_directories(platformerutils PUBLIC platformerutils/inc)
target_link_libraries(platformerutils json11 fmt::fmt)
