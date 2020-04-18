# imgui + sfml built as a lib, intentionally not using full warning flags
add_library(imgui imgui/lib/imgui.cpp imgui/lib/imgui_draw.cpp imgui/lib/imgui-SFML.cpp)
target_link_libraries(imgui PUBLIC
                                        GL
                                        CONAN_PKG::sfml
                                        )

