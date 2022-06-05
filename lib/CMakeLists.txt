project(minirdr)

add_library(mrdr STATIC
    inc/mrdr.h
    src/main.cpp
    src/tinygltf.cpp
    src/mrdr/context.h
    src/mrdr/context.cpp
    src/mrdr/dx12/common.h
    src/mrdr/dx12/command_fence.h
    src/mrdr/dx12/command_fence.cpp
    src/mrdr/dx12/command_pool.h
    src/mrdr/dx12/command_pool.cpp
    src/mrdr/dx12/command_queue.h
    src/mrdr/dx12/command_queue.cpp
    src/mrdr/dx12/descriptor_span.h
    src/mrdr/dx12/device.h
    src/mrdr/dx12/device.cpp
    src/mrdr/dx12/gpu_descriptor_heap.h
    src/mrdr/dx12/gpu_descriptor_heap.cpp
    src/mrdr/dx12/renderer.h
    src/mrdr/dx12/renderer.cpp
    src/mrdr/dx12/swap_chain.h
    src/mrdr/dx12/swap_chain.cpp
)

target_include_directories(mrdr
    PUBLIC "inc"
    PRIVATE "inc/mrdr"
)

target_link_libraries(mrdr
PUBLIC
    spdlog
    imgui 
PRIVATE
    Microsoft.GSL::GSL
    SDL2::SDL2 SDL2::SDL2main
    imgui-sdl2 imgui-dx12
    d3d12 dxgi
    tinygltf json stb
)

# target_precompile_headers(mrdr
#     PUBLIC
#     inc/mrdr/common.h
# )
