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

// detect support for AVX512
#if defined(__AVX512F__)
#define _SIMDCOPY_AVX512_ 1
#endif

// detect support for AVX
#if defined(__AVX__)
#define _SIMDCOPY_AVX_ 1
#endif

// detect support for SSE
#if defined(_MSC_VER)
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

#define SIMDCOPY_REMAINING(DEST,SRC,REMAINING)\
if (REMAINING)\
{\
    memcpy(DEST, SRC, REMAINING);\
}

#ifdef _SIMDCOPY_AVX512_
#define SIMDCOPY_BLOCK_AVX512 (sizeof(__m512))
#define SIMDCOPY_AVX512(DEST, SRC, BLOCKS) \
for (size_t offset = 0; offset < BLOCKS; offset++)\
{\
    _mm512_storeu_ps((float *)DEST + offset * (SIMDCOPY_BLOCK_AVX512 / sizeof(float)), _mm512_loadu_ps((float *)SRC + offset * (SIMDCOPY_BLOCK_AVX512 / sizeof(float))));\
}
#else
#define SIMDCOPY_BLOCK_AVX512 (1 / 0) // this is defined if AVX512 is not supported
#endif

#ifdef _SIMDCOPY_AVX_
#define SIMDCOPY_BLOCK_AVX (sizeof(__m256))
#define SIMDCOPY_AVX(DEST, SRC, BLOCKS) \
for (size_t offset = 0; offset < BLOCKS; offset++)\
{\
    _mm256_storeu_ps((float *)DEST + offset * (SIMDCOPY_BLOCK_AVX / sizeof(float)), _mm256_loadu_ps((float *)SRC + offset * (SIMDCOPY_BLOCK_AVX / sizeof(float))));\
}
#else
#define SIMDCOPY_BLOCK_AVX (1 / 0) // this is defined if AVX is not supported
#endif

#ifdef _SIMDCOPY_SSE_
#define SIMDCOPY_BLOCK_SSE (sizeof(__m128))
#define SIMDCOPY_SSE(DEST, SRC, BLOCKS) \
for (size_t offset = 0; offset < BLOCKS; offset++)\
{\
    _mm_storeu_ps((float *)DEST + offset * (SIMDCOPY_BLOCK_SSE / sizeof(float)), _mm_loadu_ps((float *)SRC + offset * (SIMDCOPY_BLOCK_SSE / sizeof(float))));\
}
#else
#define SIMDCOPY_BLOCK_SSE (1 / 0) // this is defined if SSE is not supported
#endif

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
    
    inline void *memcpy_aligned_avx512(void *dest, const void *src, size_t blocks)
    {
        void *result = NULL;
#ifdef _SIMDCOPY_AVX512_
        SIMDCOPY_AVX512(dest, src, blocks)
        result = dest;
#endif
        return result;
    }

    inline void *memcpy_avx512(void *dest, const void *src, size_t blocks)
    {
        void *result = NULL;
#ifdef _SIMDCOPY_AVX512_
        SIMDCOPY_AVX512(dest, src, blocks)
        result = dest;
#endif
        return result;
    }

    inline void *memcpy_aligned_avx(void *dest, const void *src, size_t blocks)
    {
        void *result = NULL;
#ifdef _SIMDCOPY_AVX_
        SIMDCOPY_AVX(dest, src, blocks)
        result = dest;
#endif
        return result;
    }

    inline void *memcpy_avx(void *dest, const void *src, size_t count)
    {
        void *result = NULL;
#ifdef _SIMDCOPY_AVX_
        const size_t blocks = count / SIMDCOPY_BLOCK_AVX;
        const size_t remaining = count % SIMDCOPY_BLOCK_AVX;
        SIMDCOPY_AVX(dest, src, blocks)
        SIMDCOPY_REMAINING((uint8_t *)dest + blocks * SIMDCOPY_BLOCK_AVX, (uint8_t *)src + blocks * SIMDCOPY_BLOCK_AVX, remaining)
        result = dest;
#endif
        return result;
    }

    inline void *memcpy_aligned_sse(void *dest, const void *src, size_t blocks)
    {
        void *result = NULL;
#ifdef _SIMDCOPY_SSE_
        SIMDCOPY_SSE(dest, src, blocks)
        result = dest;
#endif
        return result;
    }

    inline void *memcpy_sse(void *dest, const void *src, size_t count)
    {
        void *result = NULL;
#ifdef _SIMDCOPY_SSE_
        const size_t blocks = count / SIMDCOPY_BLOCK_SSE;
        const size_t remaining = count % SIMDCOPY_BLOCK_SSE;
        SIMDCOPY_SSE(dest, src, blocks)
        SIMDCOPY_REMAINING((uint8_t *)dest + blocks * SIMDCOPY_BLOCK_SSE, (uint8_t *)src + blocks * SIMDCOPY_BLOCK_SSE, remaining);  
        result = dest;
#endif
        return result;
    }

#ifdef __cplusplus
}
#endif

#endif