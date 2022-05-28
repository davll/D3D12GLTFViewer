project(DearImGUI_D3D12Example)

add_executable(imgui_dx12 main.cpp)

target_link_libraries(imgui_dx12 PRIVATE
    imgui d3d12 dxgi
)
