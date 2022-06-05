cmake_minimum_required(VERSION 3.16)
project(D3D12GLTFViewer)

find_package(Threads REQUIRED)
find_package(SDL2 CONFIG REQUIRED)
find_package(Microsoft.GSL CONFIG REQUIRED)

include(CheckSymbolExists)

add_subdirectory(extern)
add_subdirectory(lib)

add_executable(D3D12GLTFViewer
    main.cpp
    guifont.h
    guifont.cpp
    guifont_chicago.cpp
    guifont_ibmplexmono.cpp
    guistyle.h
    guistyle.cpp
)

target_link_libraries(D3D12GLTFViewer PRIVATE
    mrdr
)
