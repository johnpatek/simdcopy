/*
 * Copyright 2024 John R. Patek Sr.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

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

void test_memcpy_avx512()
{
#ifdef _SIMDCOPY_AVX512_
    iterate_memcpy<SIMDCOPY_BLOCK_AVX512, 1, 10, false>("test_memcpy_avx512", memcpy_avx512);
#endif
}

void test_memcpy_aligned_avx512()
{
#ifdef _SIMDCOPY_AVX512_
    iterate_memcpy<SIMDCOPY_BLOCK_AVX512, 1, 10, true>("test_memcpy_avx512", memcpy_aligned_avx512);
#endif
}

void test_memcpy_avx()
{
#ifdef _SIMDCOPY_AVX_
    iterate_memcpy<SIMDCOPY_BLOCK_AVX, 1, 10, false>("test_memcpy_avx", memcpy_avx);
#endif
}

void test_memcpy_aligned_avx()
{
#ifdef _SIMDCOPY_AVX_
    iterate_memcpy<sizeof(__m256), 1, 10, true>("test_memcpy_aligned_avx", memcpy_aligned_avx);
#endif
}

void test_memcpy_sse()
{
#ifdef _SIMDCOPY_SSE_
    iterate_memcpy<SIMDCOPY_BLOCK_AVX, 1, 10, false>("test_memcpy_sse", memcpy_sse);
#endif
}

void test_memcpy_aligned_sse()
{
#ifdef _SIMDCOPY_SSE_
    iterate_memcpy<SIMDCOPY_BLOCK_SSE, 1, 10, true>("test_memcpy_aligned_sse", memcpy_aligned_sse);
#endif
}

int main(int argc, const char **argv)
{
    const std::unordered_map<std::string, std::function<void()>> unit_tests = {
        {"MEMCPY_AVX512", test_memcpy_avx512},
        {"MEMCPY_ALIGNED_AVX512", test_memcpy_aligned_avx512},
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