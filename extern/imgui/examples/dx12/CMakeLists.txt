project(DearImGUI_Examples_DX12)

add_executable(imgui-example-dx12 main.cpp)

target_link_libraries(imgui-example-dx12 PRIVATE
    imgui imgui-sdl2 imgui-dx12
)
