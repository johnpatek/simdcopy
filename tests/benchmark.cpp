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