if(SIMDCOPY_TESTS)
    add_executable(
        unit 
        unit.cpp)
    target_link_libraries(unit simdcopy)

    if(SIMDCOPY_COVER)
        if(WIN32)
            message("skipping code coverage for windows")
        else()
            SET(CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} -fprofile-arcs -ftest-coverage -g -O0")
            SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage -g -O0")
            add_custom_target(
                cover
                DEPENDS unit)
            add_custom_command(
                TARGET cover
                COMMAND gcovr -r ${CMAKE_SOURCE_DIR} -e ${CMAKE_SOURCE_DIR}/tests)
        endif()
    endif()

    add_test(NAME test_memcpy_avx COMMAND unit MEMCPY_AVX)
    add_test(NAME test_memcpy_aligned_avx COMMAND unit MEMCPY_ALIGNED_AVX)
    add_test(NAME test_memcpy_sse COMMAND unit MEMCPY_SSE)
    add_test(NAME test_memcpy_aligned_sse COMMAND unit MEMCPY_ALIGNED_SSE)
endif()