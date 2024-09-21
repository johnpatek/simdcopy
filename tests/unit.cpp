#include <simdcopy.h>

#include <algorithm>
#include <array>
#include <functional>
#include <iostream>
#include <unordered_map>
#include <string>

template <size_t BlockSize, size_t Start, size_t End, bool Aligned>
static void iterate_memcpy(const std::string &label, const std::function<void *(void *, void *, size_t)> &copyfn)
{
    constexpr size_t ArraySize = (Start * BlockSize) + ((Aligned) ? 0 : (BlockSize / 2));
    constexpr size_t CopySize = ((Aligned) ? Start : ArraySize);
    std::array<uint8_t, ArraySize> input;
    std::array<uint8_t, ArraySize> output;
    uint8_t product;

    input.fill(1);
    output.fill(0);

    copyfn(output.data(), input.data(), CopySize);

    product = 1;
    std::for_each(
        output.cbegin(),
        output.cend(),
        [&](const uint8_t byte)
        {
            product *= byte;
        });
    if (product != 1)
    {
        throw std::runtime_error(label + " failed at size " + std::to_string(ArraySize));
    }

    if constexpr (Start < End)
    {
        iterate_memcpy<BlockSize, Start + 1, End, Aligned>(label, copyfn);
    }
}

void test_memcpy_avx()
{
    iterate_memcpy<SIMDCOPY_BLOCK_AVX, 1, 10, false>("test_memcpy_avx", memcpy_avx);
}

void test_memcpy_aligned_avx()
{
    iterate_memcpy<sizeof(__m256), 1, 10, true>("test_memcpy_aligned_avx", memcpy_aligned_avx);
}

void test_memcpy_sse()
{
    iterate_memcpy<SIMDCOPY_BLOCK_AVX, 1, 10, false>("test_memcpy_sse", memcpy_sse);
}

void test_memcpy_aligned_sse()
{
    iterate_memcpy<SIMDCOPY_BLOCK_SSE, 1, 10, true>("test_memcpy_aligned_sse", memcpy_aligned_sse);
}

int main(int argc, const char **argv)
{
    const std::unordered_map<std::string, std::function<void()>> unit_tests = {
        {"MEMCPY_AVX", test_memcpy_avx},
        {"MEMCPY_ALIGNED_AVX", test_memcpy_aligned_avx},
        {"MEMCPY_SSE", test_memcpy_sse},
        {"MEMCPY_ALIGNED_SSE", test_memcpy_aligned_sse},
    };
    int result;
    result = 0;
    if (argc > 1)
    {
        try
        {
            const std::function<void()> &unit_test = unit_tests.at(argv[1]);
            unit_test();
        }
        catch (const std::exception &exception)
        {
            std::cerr << exception.what() << std::endl;
            result = 1;
        }
    }
    return result;
}