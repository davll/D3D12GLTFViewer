project(fmt)

set(FMT_MODULE FALSE)
set(FMT_OS TRUE)

if (WIN32)
  check_symbol_exists(_strtod_l "${strtod_l_headers}" HAVE_STRTOD_L)
else ()
  check_symbol_exists(strtod_l "${strtod_l_headers}" HAVE_STRTOD_L)
endif ()

if(FMT_MODULE)
    set(FMT_SOURCES src/fmt.cc)
elseif(FMT_OS)
    set(FMT_SOURCES src/format.cc src/os.cc)
else()
    set(FMT_SOURCES src/format.cc)
endif()

add_library(fmt STATIC ${FMT_SOURCES})
target_include_directories(fmt PUBLIC include)
if(HAVE_STRTOD_L)
  target_compile_definitions(fmt PUBLIC FMT_LOCALE)
endif()
