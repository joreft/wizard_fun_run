add_library(log INTERFACE)
#target_compile_features(log PRIVATE cxx_std_17)
target_include_directories(log INTERFACE inc)
target_link_libraries(log INTERFACE spdlog::spdlog)
