project(DearImGUI_Backends_SDL2)

add_library(imgui-sdl2 STATIC
    inc/imgui_impl_sdl.h
    src/imgui_impl_sdl.cpp
)

target_include_directories(imgui-sdl2
    PUBLIC inc
)

target_link_libraries(imgui-sdl2
    PUBLIC imgui SDL2::SDL2
)
