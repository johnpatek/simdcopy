cmake_minimum_required (VERSION 3.10)

project(simdcopy)

option(SIMDCOPY_AVX512 "Build with AVX512" OFF)
option(SIMDCOPY_TESTS "Build unit tests" OFF)
option(SIMDCOPY_COVER "Add code coverage" OFF)
option(SIMDCOPY_BENCH "Build benchmark tests" OFF)

if(WIN32)
    if(SIMDCOPY_AVX512)
        set(COMPILER_FLAGS "/arch:AVX512")
    else()
        set(COMPILER_FLAGS "/arch:AVX")
    endif()
else()
    set(COMPILER_FLAGS "-march=native")
endif()

add_library(simdcopy INTERFACE)
target_include_directories(simdcopy INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_options(simdcopy INTERFACE ${COMPILER_FLAGS})

include(CTest)
enable_testing()
add_subdirectory(tests)