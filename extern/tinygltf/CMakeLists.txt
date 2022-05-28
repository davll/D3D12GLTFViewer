project(tinygltf)

add_library(tinygltf INTERFACE)
target_include_directories(tinygltf INTERFACE inc)
target_link_libraries(tinygltf INTERFACE json stb)
