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

#include "simdcopy.h"

#include <algorithm>
#include <array>
#include <chrono>
#include <functional>
#include <iomanip>
#include <iostream>
#include <list>
#include <tuple>
#include <vector>

template <size_t Base, size_t Power>
static constexpr size_t exponent()
{
    if constexpr (Power > 0)
    {
        return Base * exponent<Base, Power - 1>();
    }
    else
    {
        return 1;
    }
}

static void print_table_header(std::ostream &stream, size_t copy_size, bool aligned)
{
    stream << "size=" << copy_size << " aligned=" << ((aligned) ? "true" : "false") << std::endl;
    stream << std::string(70, '-') << std::endl;
    stream << std::left
           << "" << std::setw(22) << "FUNC"
           << " " << std::setw(15) << "MIN"
           << " " << std::setw(15) << "MAX"
           << " " << std::setw(15) << "AVG"
           << "" << std::endl;
    stream << std::string(70, '-') << std::endl;
}

static void print_table_row(std::ostream &stream, const std::string &label, const std::tuple<uint64_t, uint64_t, uint64_t> &metrics)
{
    stream << std::left
           << "" << std::setw(22) << label
           << " " << std::setw(15) << std::get<0>(metrics)
           << " " << std::setw(15) << std::get<1>(metrics)
           << " " << std::setw(15) << std::get<2>(metrics)
           << "" << std::endl;
}

static void print_table_footer(std::ostream &stream)
{
    stream << std::string(70, '-') << std::endl
           << std::endl;
}

template <size_t Size, size_t BlockSize, bool Aligned>
static std::tuple<uint64_t, uint64_t, uint64_t> profile_function(const std::function<void *(void *, void *, size_t)> &func, std::vector<uint8_t> &input, std::vector<uint8_t> &output)
{
    constexpr size_t Count = Size / (Aligned ? BlockSize : 1);
    std::array<std::chrono::nanoseconds, 100> times;
    uint8_t *const src = input.data();
    uint8_t *const dest = output.data();
    uint64_t min,max,sum,avg;

    for (size_t index = 0; index < 100; index++)
    {
        auto start = std::chrono::high_resolution_clock::now();
        func(dest, src, Count);
        auto end = std::chrono::high_resolution_clock::now();
        times[index] = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    }

    std::sort(times.begin(), times.end());
    min = static_cast<uint64_t>(times.front().count());
    max = static_cast<uint64_t>(times.back().count());

    sum = 0;
    for(const auto& time : times)
    {
        sum += static_cast<uint64_t>(time.count());
    }
    avg = sum / 100;

    return std::make_tuple(min, max, avg);
}

template <size_t Size, bool Aligned>
static void benchmark(std::ostream &stream)
{
    std::vector<uint8_t> input(Size);
    std::vector<uint8_t> output(Size);
    print_table_header(stream, Size, Aligned);
    print_table_row(stream, "memcpy", profile_function<Size, 1, false>(::memcpy, input, output));
    if constexpr (Aligned)
    {
        print_table_row(stream, "memcpy_aligned_sse", profile_function<Size, SIMDCOPY_BLOCK_SSE, true>(::memcpy_aligned_sse, input, output));
        print_table_row(stream, "memcpy_aligned_avx", profile_function<Size, SIMDCOPY_BLOCK_AVX, true>(::memcpy_aligned_avx, input, output));
    }
    else
    {
        print_table_row(stream, "memcpy_sse", profile_function<Size, SIMDCOPY_BLOCK_SSE, false>(::memcpy_sse, input, output));
        print_table_row(stream, "memcpy_avx", profile_function<Size, SIMDCOPY_BLOCK_AVX, false>(::memcpy_avx, input, output));
    }
    print_table_footer(stream);
}

template <size_t Start, size_t End, bool Aligned>
static void iterate_benchmarks(std::ostream &stream)
{
    constexpr size_t CopySize = 1024 * exponent<10, Start>() + (Aligned ? 0 : 64);

    if constexpr (Aligned)
    {
        benchmark<CopySize, Aligned>(stream);
        iterate_benchmarks<Start, End, false>(stream);
    }
    else
    {
        benchmark<CopySize, Aligned>(stream);
        if constexpr (Start < End)
        {
            iterate_benchmarks<Start + 1, End, true>(stream);
        }
    }
}

int main()
{
    iterate_benchmarks<0, 5, true>(std::cerr);
    return 0;
}