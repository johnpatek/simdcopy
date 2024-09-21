add_executable(
    unit 
    unit.cpp)
target_link_libraries(unit simdcopy)

add_test(NAME test_memcpy_avx COMMAND unit MEMCPY_AVX)
add_test(NAME test_memcpy_aligned_avx COMMAND unit MEMCPY_ALIGNED_AVX)
add_test(NAME test_memcpy_sse COMMAND unit MEMCPY_SSE)
add_test(NAME test_memcpy_aligned_sse COMMAND unit MEMCPY_ALIGNED_SSE)