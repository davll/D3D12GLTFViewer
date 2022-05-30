project(spdlog)

set(SPDLOG_SRCS
    src/spdlog.cpp
    src/stdout_sinks.cpp
    src/color_sinks.cpp
    src/file_sinks.cpp
    src/async.cpp
    src/cfg.cpp
)

add_library(spdlog STATIC ${SPDLOG_SRCS})
target_compile_definitions(spdlog PUBLIC
    SPDLOG_COMPILED_LIB
    SPDLOG_FMT_EXTERNAL
)
target_include_directories(spdlog PUBLIC include)
target_link_libraries(spdlog PUBLIC fmt Threads::Threads)
