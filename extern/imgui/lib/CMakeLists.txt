project(DearImGUI)

add_library(imgui STATIC
    inc/imconfig.h
    inc/imgui.h
    src/imgui_demo.cpp
    src/imgui_draw.cpp
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
