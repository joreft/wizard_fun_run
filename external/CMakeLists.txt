add_library(imgui imgui/lib/imgui.cpp imgui/lib/imgui_draw.cpp imgui/lib/imgui-SFML.cpp)
target_include_directories(imgui INTERFACE imgui/lib)

target_compile_features(imgui PRIVATE cxx_std_17)
target_link_libraries(imgui PUBLIC  OpenGL::GL
                                    CONAN_PKG::sfml)

add_library(json11 json11/src/json11.cpp)
target_include_directories(json11 PUBLIC json11/inc)

