add_library(wizard_fun_run src/physics.cpp)
target_compile_features(wizard_fun_run PRIVATE cxx_std_17)
target_include_directories(wizard_fun_run PUBLIC inc)
target_link_libraries(wizard_fun_run platformerutils log json11 fmt::fmt)
