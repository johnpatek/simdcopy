#ifndef _SMDCOPY_HPP_
#define _SMDCOPY_HPP_

#include "simdcopy.h"

namespace simdcopy
{
    inline void copy_bytes_avx(uint8_t *dest, uint8_t *src, size_t count)
    {
#ifdef _SIMDCOPY_AVX_
        const size_t blocks = count / SIMDCOPY_BLOCK_AVX;
        const size_t remaining = count % SIMDCOPY_BLOCK_AVX;
        SIMDCOPY_AVX(dest, src, blocks)
        SIMDCOPY_REMAINING((uint8_t *)dest + blocks * SIMDCOPY_BLOCK_AVX, (uint8_t *)src + blocks * SIMDCOPY_BLOCK_AVX, remaining)
#endif
    }

    template <size_t Count>
    inline void copy_bytes_avx(uint8_t *dest, uint8_t *src)
    {
#ifdef _SIMDCOPY_AVX_
        constexpr size_t blocks = Count / SIMDCOPY_BLOCK_AVX;
        constexpr size_t remaining = Count % SIMDCOPY_BLOCK_AVX;
        SIMDCOPY_AVX(dest, src, blocks)
        if constexpr (remaining)
        {
            reinterpret_cast<void>(::memcpy((uint8_t *)dest + blocks * SIMDCOPY_BLOCK_AVX, (uint8_t *)src + blocks * SIMDCOPY_BLOCK_AVX, remaining));
        }
#endif
    }

    inline void copy_bytes_sse(uint8_t *dest, uint8_t *src, size_t count)
    {
#ifdef _SIMDCOPY_SSE_
        const size_t blocks = count / SIMDCOPY_BLOCK_SSE;
        const size_t remaining = count % SIMDCOPY_BLOCK_SSE;
        SIMDCOPY_SSE(dest, src, blocks)
        SIMDCOPY_REMAINING((uint8_t *)dest + blocks * SIMDCOPY_BLOCK_SSE, (uint8_t *)src + blocks * SIMDCOPY_BLOCK_SSE, remaining);
#endif
    }

    template <size_t Count>
    inline void copy_bytes_sse(uint8_t *dest, uint8_t *src)
    {
#ifdef _SIMDCOPY_SSE_
        constexpr size_t blocks = Count / SIMDCOPY_BLOCK_SSE;
        constexpr size_t remaining = Count % SIMDCOPY_BLOCK_SSE;
        SIMDCOPY_SSE(dest, src, blocks)
        if constexpr (remaining)
        {
            reinterpret_cast<void>(::memcpy((uint8_t *)dest + blocks * SIMDCOPY_BLOCK_SSE, (uint8_t *)src + blocks * SIMDCOPY_BLOCK_SSE, remaining));
        }
#endif
    }
}
#endif