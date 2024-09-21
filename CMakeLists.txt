cmake_minimum_required (VERSION 3.10)

project(simdcopy)

add_library(simdcopy INTERFACE)

target_include_directories(simdcopy INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(tests)