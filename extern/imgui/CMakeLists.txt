project(DearImGUI)

add_library(imgui STATIC
    inc/imconfig.h
    inc/imgui.h
    inc/imgui_impl_dx12.h
    inc/imgui_impl_sdl.h
    inc/imgui_impl_win32.h
    src/imgui_demo.cpp
    src/imgui_draw.cpp
    src/imgui_impl_dx12.cpp
    src/imgui_impl_sdl.cpp
    src/imgui_impl_win32.cpp
    src/imgui_internal.h
    src/imgui_tables.cpp
    src/imgui_widgets.cpp
    src/imgui.cpp
    src/imstb_rectpack.h
    src/imstb_textedit.h
    src/imstb_truetype.h
)

target_include_directories(imgui
    PUBLIC inc
)

target_link_libraries(imgui
    PUBLIC SDL2::SDL2
)
