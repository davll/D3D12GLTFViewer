project(minirdr)

add_library(mrdr STATIC
    inc/mrdr/common.h
    inc/mrdr/Renderer.h
    src/Renderer.cpp
    src/Renderer/CommandPool.h
    src/Renderer/CommandPool.cpp
    src/Renderer/CommandQueue.h
    src/Renderer/CommandQueue.cpp
    src/Renderer/ShaderDescriptorHeap.h
    src/Renderer/ShaderDescriptorHeap.cpp
    src/Renderer/StaticDescriptors.h
    src/Renderer/SwapChain.h
    src/Renderer/SwapChain.cpp
    src/tinygltf.cpp
)

target_include_directories(mrdr
    PUBLIC "inc"
    PRIVATE "inc/mrdr"
)

target_link_libraries(mrdr
    PUBLIC
    spdlog::spdlog
    SDL2::SDL2
    imgui imgui-sdl2 imgui-dx12
    tinygltf json stb
    d3d12 dxgi
)

target_precompile_headers(mrdr
    PUBLIC
    inc/mrdr/common.h
)
