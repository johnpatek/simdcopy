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

#include <functional>
#include <iomanip>
#include <iostream>

template <size_t Base, size_t Power>
static size_t exponent()
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

}

template<size_t Size, bool Aligned>
static void benchmark(std::ostream &stream)
{
    
}

template <size_t Start, size_t End, size_t Factor, bool Aligned>
static void iterate_benchmarks(std::ostream &stream)
{
    constexpr size_t CopySize = exponent<1024,Start> * exponent<10,Factor> + (Aligned?0:64);    
}

int main()
{
    std::cerr << "s";
    return 0;
}