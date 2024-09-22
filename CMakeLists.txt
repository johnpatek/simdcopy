cmake_minimum_required (VERSION 3.10)

project(simdcopy)

option(SIMDCOPY_TESTS "Build test suite" OFF)
option(SIMDCOPY_COVER "Add code coverage" OFF)

add_library(simdcopy INTERFACE)
target_include_directories(simdcopy INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
if(MSVC)
    target_compile_options(simdcopy INTERFACE /arch:AVX2)
else()
    target_compile_options(simdcopy INTERFACE -march=native -O3)    
endif()

include(CTest)
enable_testing()
add_subdirectory(tests)