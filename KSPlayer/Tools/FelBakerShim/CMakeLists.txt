cmake_minimum_required(VERSION 3.20)
project(KSPlayerFelBakerShim LANGUAGES CXX)

option(KSPLAYER_FELBAKER_REAL_DIRECT "Build the private direct FelBaker/libdovi compositor" OFF)
set(KSPLAYER_FELBAKER_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/external/felbaker" CACHE PATH "FelBaker source checkout")
set(KSPLAYER_LIBDOVI_INCLUDE "" CACHE PATH "Directory containing libdovi rpu_parser.h")
set(KSPLAYER_LIBDOVI_LIBRARY "" CACHE FILEPATH "Path to libdovi static or dynamic library")

add_library(KSPlayerFelBakerShim SHARED
    src/KSPlayerFelBakerShim.cpp
)

target_compile_features(KSPlayerFelBakerShim PRIVATE cxx_std_20)
target_compile_options(KSPlayerFelBakerShim PRIVATE -Wall -Wextra -fvisibility=hidden)
target_include_directories(KSPlayerFelBakerShim PRIVATE ../../Documents)
target_link_libraries(KSPlayerFelBakerShim PRIVATE "-framework CoreMedia" "-framework CoreVideo")
set_target_properties(KSPlayerFelBakerShim PROPERTIES
    OUTPUT_NAME KSPlayerFelBakerShim
    INSTALL_NAME_DIR "@rpath"
)

if(KSPLAYER_FELBAKER_REAL_DIRECT)
    if(NOT KSPLAYER_LIBDOVI_INCLUDE OR NOT KSPLAYER_LIBDOVI_LIBRARY)
        message(FATAL_ERROR "Real direct builds require KSPLAYER_LIBDOVI_INCLUDE and KSPLAYER_LIBDOVI_LIBRARY")
    endif()

    target_sources(KSPlayerFelBakerShim PRIVATE
        src/KSPlayerFelBakerRealCompositor.cpp
        src/KSPlayerFelBakerDirectCore.cpp
        "${KSPLAYER_FELBAKER_ROOT}/src/RpuProcessor.cpp"
    )
    target_compile_definitions(KSPlayerFelBakerShim PRIVATE
        KSPLAYER_FELBAKER_ENABLE_REAL_COMPOSITOR=1
        KSPLAYER_FELBAKER_HAS_CORE_ADAPTER=1
    )
    target_include_directories(KSPlayerFelBakerShim PRIVATE
        src
        "${KSPLAYER_FELBAKER_ROOT}/src"
        "${KSPLAYER_LIBDOVI_INCLUDE}"
    )
    target_link_libraries(KSPlayerFelBakerShim PRIVATE "${KSPLAYER_LIBDOVI_LIBRARY}")
endif()

add_executable(smoke_load tests/smoke_load.cpp)
target_compile_features(smoke_load PRIVATE cxx_std_20)
target_include_directories(smoke_load PRIVATE ../../Documents)
target_link_libraries(smoke_load PRIVATE "-framework CoreMedia" "-framework CoreVideo")
