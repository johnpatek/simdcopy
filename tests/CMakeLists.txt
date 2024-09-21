add_executable(
    unit 
    unit.cpp)
target_link_libraries(unit simdcopy)

if(MSVC)
    target_compile_options(unit PRIVATE /arch:AVX2)
endif()

add_test(NAME test_memcpy_avx COMMAND unit MEMCPY_AVX)
add_test(NAME test_memcpy_sse COMMAND unit MEMCPY_SSE)