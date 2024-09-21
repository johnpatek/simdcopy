#include <simdcopy.h>

#include <algorithm>
#include <array>
#include <functional>
#include <iostream>
#include <unordered_map>
#include <string>

static void test_memcpy_avx();
static void test_memcpy_sse();

template <const char *Label, size_t BlockSize, size_t Start, size_t End, bool Remainder>
static void iterate_sizes(void *(*copyfn)(void *, void *, size_t))
{
    constexpr size_t ArraySize = (Start * BlockSize) + ((Remainder) ? (BlockSize / 2) : 0);
    std::array<uint8_t, ArraySize> input;
    std::array<uint8_t, ArraySize> output;
    uint8_t product;

    input.fill(1);
    input.fill(0);

    if constexpr (!Remainder)
    {
        iterate_sizes<BlockSize, Start, End, true>();
    }

    if constexpr (Start < End)
    {
        iterate_sizes<BlockSize, Start + 1, End, false>();
    }

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
        throw std::runtime_error(std::string(Label) + " failed at size " + std::to_string(ArraySize));
    }
}

int main(int argc, const char **argv)
{
    const std::unordered_map<std::string, std::function<void()>> unit_tests = {
        {"MEMCPY_AVX", test_memcpy_avx},
        {"MEMCPY_SSE", test_memcpy_sse},
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

void test_memcpy_avx()
{
    memcpy_avx(NULL, NULL, 0);
}

void test_memcpy_sse()
{
}