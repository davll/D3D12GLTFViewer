project(DearImGUI_Examples_DX12)

add_executable(imgui-example-dx12 WIN32 main.cpp)

target_link_libraries(imgui-example-dx12 PRIVATE
    imgui imgui-sdl2 imgui-dx12 SDL2::SDL2 SDL2::SDL2main
)
