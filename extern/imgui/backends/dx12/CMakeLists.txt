project(DearImGUI_Backends_DX12)

add_library(imgui-dx12 STATIC
    inc/imgui_impl_dx12.h
    src/imgui_impl_dx12.cpp
)

target_include_directories(imgui-dx12
    PUBLIC inc
)

target_link_libraries(imgui-dx12
    PUBLIC imgui d3d12 dxgi
)
