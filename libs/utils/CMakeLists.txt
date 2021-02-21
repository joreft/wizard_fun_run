add_library(utils src/json11_from_file.cpp)
target_compile_features(utils PRIVATE cxx_std_17)
target_include_directories(utils PUBLIC inc)
target_link_libraries(utils json11 fmt::fmt platformerutils log)
