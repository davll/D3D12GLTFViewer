cmake_minimum_required(VERSION 3.16)
project(D3D12GLTFViewer)

find_package(SDL2 CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)

add_subdirectory(extern)
add_subdirectory(lib)

add_executable(D3D12GLTFViewer
    main.cpp
    font.h
    font_chicago.cpp
    font_ibmplexmono.cpp
)

target_link_libraries(D3D12GLTFViewer PRIVATE
    mrdr SDL2::SDL2 SDL2::SDL2main
)
