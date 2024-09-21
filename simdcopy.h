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

#ifndef _SIMDCOPY_H_
#define _SIMDCOPY_H_

#ifdef __cplusplus
extern "C"
{
#endif

#if defined(_MSC_VER)
/* Microsoft C/C++-compatible compiler */
#include <intrin.h>
#elif defined(__GNUC__) && (defined(__x86_64__) || defined(__i386__))
/* GCC-compatible compiler, targeting x86/x86-64 */
#include <x86intrin.h>
#elif defined(__GNUC__) && defined(__ARM_NEON__)
/* GCC-compatible compiler, targeting ARM with NEON */
#include <arm_neon.h>
#elif defined(__GNUC__) && defined(__IWMMXT__)
/* GCC-compatible compiler, targeting ARM with WMMX */
#include <mmintrin.h>
#elif (defined(__GNUC__) || defined(__xlC__)) && (defined(__VEC__) || defined(__ALTIVEC__))
/* XLC or GCC-compatible compiler, targeting PowerPC with VMX/VSX */
#include <altivec.h>
#elif defined(__GNUC__) && defined(__SPE__)
/* GCC-compatible compiler, targeting PowerPC with SPE */
#include <spe.h>
#endif

#if defined(_MSC_VER)
/**/ #if defined(__AVX__)
/**/ #define _SIMDCOPY_AVX_ 1
/**/ #endif
/**/ #if (defined(_M_AMD64) || defined(_M_X64))
/**/ #define _SIMDCOPY_SSE_ 1
/**/ #endif
#else
/**/ #if defined(__AVX__)
/**/ #define _SIMDCOPY_AVX_ 1
/**/ #endif
/**/ #if defined(__SSE__)
/**/ #define _SIMDCOPY_SSE_ 1
/**/ #endif
#endif

#ifdef _SIMDCOPY_AVX_
#define SIMDCOPY_BLOCK_AVX (sizeof(__m256))
#else
#define SIMDCOPY_BLOCK_AVX (1 / 0) // this is defined if AVX is not supported
#endif

#ifdef _SIMDCOPY_SSE_
#define SIMDCOPY_BLOCK_SSE (sizeof(__m128))
#else
#define SIMDCOPY_BLOCK_SSE (1 / 0) // this is defined if SSE is not supported
#endif

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

    inline void *memcpy_aligned_avx(void *dest, const void *src, size_t blocks)
    {
        void *result = NULL;
#ifdef _SIMDCOPY_AVX_
        for (size_t offset = 0; offset < blocks; offset++)
        {
            _mm256_storeu_ps((float *)dest + offset * (SIMDCOPY_BLOCK_AVX / sizeof(float)), _mm256_loadu_ps((float *)src + offset * (SIMDCOPY_BLOCK_AVX / sizeof(float))));
        }
        result = dest;
#endif
        return result;
    }

    inline void *memcpy_avx(void *dest, const void *src, size_t count)
    {
        void *result = NULL;
#ifdef _SIMDCOPY_AVX_
        const size_t blocks = count / SIMDCOPY_BLOCK_AVX;
        const size_t remainder = count % SIMDCOPY_BLOCK_AVX;
        for (size_t offset = 0; offset < blocks; offset++)
        {
            _mm256_storeu_ps((float *)dest + offset * (SIMDCOPY_BLOCK_AVX / sizeof(float)), _mm256_loadu_ps((float *)src + offset * (SIMDCOPY_BLOCK_AVX / sizeof(float))));
        }
        if (remainder)
        {
            (void)memcpy((uint8_t *)dest + blocks * SIMDCOPY_BLOCK_AVX, (uint8_t *)src + blocks * SIMDCOPY_BLOCK_AVX, remainder);
        }
        result = dest;
#endif
        return result;
    }

    inline void *memcpy_aligned_sse(void *dest, const void *src, size_t blocks)
    {
        void *result = NULL;
#ifdef _SIMDCOPY_SSE_
        for (size_t offset = 0; offset < blocks; offset++)
        {
            _mm_storeu_ps((float *)dest + offset * (SIMDCOPY_BLOCK_SSE / sizeof(float)), _mm_loadu_ps((float *)src + offset * (SIMDCOPY_BLOCK_SSE / sizeof(float))));
        }
        result = dest;
#endif
        return result;
    }

    inline void *memcpy_sse(void *dest, const void *src, size_t count)
    {
        void *result = NULL;
#ifdef _SIMDCOPY_SSE_
        const size_t blocks = count / SIMDCOPY_BLOCK_SSE;
        const size_t remainder = count % SIMDCOPY_BLOCK_SSE;
        for (size_t offset = 0; offset < blocks; offset++)
        {
            _mm_storeu_ps((float *)dest + offset * (SIMDCOPY_BLOCK_SSE / sizeof(float)), _mm_loadu_ps((float *)src + offset * (SIMDCOPY_BLOCK_SSE / sizeof(float))));
        }
        if (remainder)
        {
            (void)memcpy((uint8_t *)dest + blocks * SIMDCOPY_BLOCK_SSE, (uint8_t *)src + blocks * SIMDCOPY_BLOCK_SSE, remainder);
        }
        result = dest;
#endif
        return result;
    }

#ifdef __cplusplus
}
#endif

#endif