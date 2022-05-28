project(minirdr)

add_library(minirdr STATIC
    inc/minirdr/common.h
    inc/minirdr/Renderer.h
    src/minirdr.cpp
    src/Renderer.cpp
    src/Renderer/CommandManager.h
    src/Renderer/CommandManager.cpp
    src/Renderer/ShaderDescriptorHeap.h
    src/Renderer/ShaderDescriptorHeap.cpp
    src/Renderer/SwapChain.h
    src/Renderer/SwapChain.cpp
)

target_include_directories(minirdr
    PUBLIC "inc"
    PRIVATE "inc/minirdr"
)

target_link_libraries(minirdr
    PUBLIC
    spdlog::spdlog
    SDL2::SDL2
    imgui
    d3d12 dxgi
)

target_precompile_headers(minirdr
    PUBLIC
    inc/minirdr/common.h
)
