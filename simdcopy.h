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
#endif

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

    inline void *memcpy_avx(void *dest, const void *src, size_t count)
    {
        void *result = NULL;
#ifdef _SIMDCOPY_AVX_
        const size_t blocks = count / sizeof(__m256);
        const size_t remainder = count % sizeof(__m256);
        for (size_t offset = 0; offset < blocks; offset++)
        {
            _mm256_storeu_ps((float *)dest + offset * (sizeof(__m256) / sizeof(float)), _mm256_loadu_ps((float *)src + offset * (sizeof(__m256) / sizeof(float))));
        }
        if (remainder)
        {
            (void)memcpy((uint8_t *)src + blocks * sizeof(__m256), (uint8_t *)dest + blocks * sizeof(__m256), remainder);
        }
        result = dest;
#endif
        return result;
    }


    inline void *memcpy_sse(void *dest, const void *src, size_t count)
    {
        void *result = NULL;
#ifdef _SIMDCOPY_SSE_
        const size_t blocks = count / sizeof(__m128);
        const size_t remainder = count % sizeof(__m128);
        for (size_t offset = 0; offset < blocks; offset++)
        {
            _mm_storeu_ps((float *)dest + offset * (sizeof(__m128) / sizeof(float)), _mm_loadu_ps((float *)src + offset * (sizeof(__m128) / sizeof(float))));
        }
        if (remainder)
        {
            (void)memcpy((uint8_t *)src + blocks * sizeof(__m128), (uint8_t *)dest + blocks * sizeof(__m128), remainder);
        }
        result = dest;
#endif
        return result;
    }

#ifdef __cplusplus
}
#endif

#endif