#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <string.h>

#include <x86intrin.h>

#include "avx512f_hamming_weight.h"


// ---------------


static uint64_t __attribute__((always_inline)) _mm512_popcnt(const __m512i v) {

    uint64_t tmp[8] __attribute__((aligned(64)));

    _mm512_store_si512(tmp, v);

    return _mm_popcnt_u64(tmp[0])
         + _mm_popcnt_u64(tmp[1])
         + _mm_popcnt_u64(tmp[2])
         + _mm_popcnt_u64(tmp[3])
         + _mm_popcnt_u64(tmp[4])
         + _mm_popcnt_u64(tmp[5])
         + _mm_popcnt_u64(tmp[6])
         + _mm_popcnt_u64(tmp[7]);
}


// ------------------------------


static uint64_t popcnt_harley_seal__hardware_popcnt_2(const __m512i* data, const uint64_t size)
{
  uint64_t total = 0;
  __m512i ones      = _mm512_setzero_si512();
  __m512i twos      = _mm512_setzero_si512();
  __m512i fours     = _mm512_setzero_si512();
  __m512i eights    = _mm512_setzero_si512();
  __m512i sixteens  = _mm512_setzero_si512();
  __m512i thirtytwos = _mm512_setzero_si512();

  const uint64_t limit = size - size % 16;
  uint64_t i = 0;

  uint64_t tmp[8] __attribute__((aligned(64)));

  for(; i < limit; i += 32)
  {
    /*
    
    inline, naive assembly -- the first attempt, translation of following code
    (now doubled, to handle thirtytwos)

        CSA(&t0, &ones, ones, data[i+0], data[i+1]);
        CSA(&t1, &ones, ones, data[i+2], data[i+3]);
        CSA(&t2, &ones, ones, data[i+4], data[i+5]);
        CSA(&t3, &ones, ones, data[i+6], data[i+7]);
        CSA(&t4, &ones, ones, data[i+8], data[i+9]);
        CSA(&t5, &ones, ones, data[i+10], data[i+11]);
        CSA(&t6, &ones, ones, data[i+12], data[i+13]);
        CSA(&t7, &ones, ones, data[i+14], data[i+15]);

        CSA(&t0, &twos, twos, t0, t1);
        CSA(&t2, &twos, twos, t2, t3);
        CSA(&t4, &twos, twos, t4, t5);
        CSA(&t6, &twos, twos, t6, t7);

        CSA(&t0, &fours, fours, t0, t2);
        CSA(&t4, &fours, fours, t4, t6);
        
        CSA(&sixteens, &eights, eights, t0, t4);
    
    There are three asm statements, I couldn't put everything into
    one statement due to error: "error: more than 30 operands in ‘asm’".
    */

    uint64_t block_total;

    __asm__ volatile (
        "vmovdqa64      0x0000(%[data]), %%zmm30                        \n"
        "vmovdqa64      0x0040(%[data]), %%zmm31                        \n"
        "vmovdqa64      %[ones], %%zmm10                                \n"
        "vpternlogd     $0x96, %%zmm30, %%zmm31, %[ones]                \n"
        "vpternlogd     $0xe8, %%zmm30, %%zmm31, %%zmm10                \n"

        // store tmp
        "vmovdqa64      %[thirtytwos], (%[tmp])                         \n"

        "vmovdqa64      0x0080(%[data]), %%zmm30                        \n"
        "vmovdqa64      0x00c0(%[data]), %%zmm31                        \n"
        "vmovdqa64      %[ones], %%zmm11                                \n"
        "vpternlogd     $0x96, %%zmm30, %%zmm31, %[ones]                \n"
        "vpternlogd     $0xe8, %%zmm30, %%zmm31, %%zmm11                \n"

        "vmovdqa64      0x0100(%[data]), %%zmm30                        \n"
        "vmovdqa64      0x0140(%[data]), %%zmm31                        \n"
        "vmovdqa64      %[ones], %%zmm12                                \n"
        "vpternlogd     $0x96, %%zmm30, %%zmm31, %[ones]                \n"
        "vpternlogd     $0xe8, %%zmm30, %%zmm31, %%zmm12                \n"

        "vmovdqa64      0x0180(%[data]), %%zmm30                        \n"
        "vmovdqa64      0x01c0(%[data]), %%zmm31                        \n"
        "vmovdqa64      %[ones], %%zmm13                                \n"
        "vpternlogd     $0x96, %%zmm30, %%zmm31, %[ones]                \n"
        "vpternlogd     $0xe8, %%zmm30, %%zmm31, %%zmm13                \n"

        "vmovdqa64      0x0200(%[data]), %%zmm30                        \n"
        "vmovdqa64      0x0240(%[data]), %%zmm31                        \n"
        "vmovdqa64      %[ones], %%zmm14                                \n"
        "vpternlogd     $0x96, %%zmm30, %%zmm31, %[ones]                \n"
        "vpternlogd     $0xe8, %%zmm30, %%zmm31, %%zmm14                \n"

        "vmovdqa64      0x0280(%[data]), %%zmm30                        \n"
        "vmovdqa64      0x02c0(%[data]), %%zmm31                        \n"
        "vmovdqa64      %[ones], %%zmm15                                \n"
        "vpternlogd     $0x96, %%zmm30, %%zmm31, %[ones]                \n"
        "vpternlogd     $0xe8, %%zmm30, %%zmm31, %%zmm15                \n"

        "vmovdqa64      0x0300(%[data]), %%zmm30                        \n"
        "vmovdqa64      0x0340(%[data]), %%zmm31                        \n"
        "vmovdqa64      %[ones], %%zmm16                                \n"
        "vpternlogd     $0x96, %%zmm30, %%zmm31, %[ones]                \n"
        "vpternlogd     $0xe8, %%zmm30, %%zmm31, %%zmm16                \n"

        "vmovdqa64      0x0380(%[data]), %%zmm30                        \n"
        "vmovdqa64      0x03c0(%[data]), %%zmm31                        \n"
        "vmovdqa64      %[ones], %%zmm17                                \n"
        "vpternlogd     $0x96, %%zmm30, %%zmm31, %[ones]                \n"
        "vpternlogd     $0xe8, %%zmm30, %%zmm31, %%zmm17                \n"

        "vmovdqa64      0x0400(%[data]), %%zmm30                        \n"
        "vmovdqa64      0x0440(%[data]), %%zmm31                        \n"
        "vmovdqa64      %[ones], %%zmm18                                \n"
        "vpternlogd     $0x96, %%zmm30, %%zmm31, %[ones]                \n"
        "vpternlogd     $0xe8, %%zmm30, %%zmm31, %%zmm18                \n"

        "vmovdqa64      0x0480(%[data]), %%zmm30                        \n"
        "vmovdqa64      0x04c0(%[data]), %%zmm31                        \n"
        "vmovdqa64      %[ones], %%zmm19                                \n"
        "vpternlogd     $0x96, %%zmm30, %%zmm31, %[ones]                \n"
        "vpternlogd     $0xe8, %%zmm30, %%zmm31, %%zmm19                \n"

        "vmovdqa64      0x0500(%[data]), %%zmm30                        \n"
        "vmovdqa64      0x0540(%[data]), %%zmm31                        \n"
        "vmovdqa64      %[ones], %%zmm20                                \n"
        "vpternlogd     $0x96, %%zmm30, %%zmm31, %[ones]                \n"
        "vpternlogd     $0xe8, %%zmm30, %%zmm31, %%zmm20                \n"

        "vmovdqa64      0x0580(%[data]), %%zmm30                        \n"
        "vmovdqa64      0x05c0(%[data]), %%zmm31                        \n"
        "vmovdqa64      %[ones], %%zmm21                                \n"
        "vpternlogd     $0x96, %%zmm30, %%zmm31, %[ones]                \n"
        "vpternlogd     $0xe8, %%zmm30, %%zmm31, %%zmm21                \n"

        "vmovdqa64      0x0600(%[data]), %%zmm30                        \n"
        "vmovdqa64      0x0640(%[data]), %%zmm31                        \n"
        "vmovdqa64      %[ones], %%zmm22                                \n"
        "vpternlogd     $0x96, %%zmm30, %%zmm31, %[ones]                \n"
        "vpternlogd     $0xe8, %%zmm30, %%zmm31, %%zmm22                \n"

        "vmovdqa64      0x0680(%[data]), %%zmm30                        \n"
        "vmovdqa64      0x06c0(%[data]), %%zmm31                        \n"
        "vmovdqa64      %[ones], %%zmm23                                \n"
        "vpternlogd     $0x96, %%zmm30, %%zmm31, %[ones]                \n"
        "vpternlogd     $0xe8, %%zmm30, %%zmm31, %%zmm23                \n"

        "vmovdqa64      0x0700(%[data]), %%zmm30                        \n"
        "vmovdqa64      0x0740(%[data]), %%zmm31                        \n"
        "vmovdqa64      %[ones], %%zmm24                                \n"
        "vpternlogd     $0x96, %%zmm30, %%zmm31, %[ones]                \n"
        "vpternlogd     $0xe8, %%zmm30, %%zmm31, %%zmm24                \n"

        "vmovdqa64      0x0780(%[data]), %%zmm30                        \n"
        "vmovdqa64      0x07c0(%[data]), %%zmm31                        \n"
        "vmovdqa64      %[ones], %%zmm25                                \n"
        "vpternlogd     $0x96, %%zmm30, %%zmm31, %[ones]                \n"
        "vpternlogd     $0xe8, %%zmm30, %%zmm31, %%zmm25                \n"

        // twos
        "vmovdqa64      %[twos], %%zmm30                                \n"
        "vpternlogd     $0x96, %%zmm10, %%zmm11, %[twos]                \n"
        "vpternlogd     $0xe8, %%zmm30, %%zmm11, %%zmm10                \n"

        "vmovdqa64      %[twos], %%zmm30                                \n"
        "vpternlogd     $0x96, %%zmm12, %%zmm13, %[twos]                \n"
        "vpternlogd     $0xe8, %%zmm30, %%zmm13, %%zmm12                \n"

        "vmovdqa64      %[twos], %%zmm30                                \n"
        "vpternlogd     $0x96, %%zmm14, %%zmm15, %[twos]                \n"
        "vpternlogd     $0xe8, %%zmm30, %%zmm15, %%zmm14                \n"

        "vmovdqa64      %[twos], %%zmm30                                \n"
        "vpternlogd     $0x96, %%zmm16, %%zmm17, %[twos]                \n"
        "vpternlogd     $0xe8, %%zmm30, %%zmm17, %%zmm16                \n"

        "vmovdqa64      %[twos], %%zmm30                                \n"
        "vpternlogd     $0x96, %%zmm18, %%zmm19, %[twos]                \n"
        "vpternlogd     $0xe8, %%zmm30, %%zmm19, %%zmm18                \n"

        "vmovdqa64      %[twos], %%zmm30                                \n"
        "vpternlogd     $0x96, %%zmm20, %%zmm21, %[twos]                \n"
        "vpternlogd     $0xe8, %%zmm30, %%zmm21, %%zmm20                \n"

        "vmovdqa64      %[twos], %%zmm30                                \n"
        "vpternlogd     $0x96, %%zmm22, %%zmm23, %[twos]                \n"
        "vpternlogd     $0xe8, %%zmm30, %%zmm23, %%zmm22                \n"

        "vmovdqa64      %[twos], %%zmm30                                \n"
        "vpternlogd     $0x96, %%zmm24, %%zmm25, %[twos]                \n"
        "vpternlogd     $0xe8, %%zmm30, %%zmm25, %%zmm24                \n"

#if 1
        "popcnt         0x00(%[tmp]), %%r8                              \n"
        "popcnt         0x08(%[tmp]), %%r9                              \n"
        "popcnt         0x10(%[tmp]), %%r10                             \n"
        "popcnt         0x18(%[tmp]), %%r11                             \n"
        "popcnt         0x20(%[tmp]), %%r12                             \n"
        "popcnt         0x28(%[tmp]), %%r13                             \n"
        "popcnt         0x30(%[tmp]), %%r14                             \n"
        "popcnt         0x38(%[tmp]), %%r15                             \n"

        "xorq           %[total], %[total]                              \n"
        "addq           %%r8,  %[total]                                 \n"
        "addq           %%r9,  %[total]                                 \n"
        "addq           %%r10, %[total]                                 \n"
        "addq           %%r11, %[total]                                 \n"
        "addq           %%r12, %[total]                                 \n"
        "addq           %%r13, %[total]                                 \n"
        "addq           %%r14, %[total]                                 \n"
        "addq           %%r15, %[total]                                 \n"
#endif

        // fours
        "vmovdqa64      %[fours], %%zmm30                               \n"
        "vpternlogd     $0x96, %%zmm10, %%zmm12, %[fours]               \n"
        "vpternlogd     $0xe8, %%zmm30, %%zmm12, %%zmm10                \n"

        "vmovdqa64      %[fours], %%zmm30                               \n"
        "vpternlogd     $0x96, %%zmm14, %%zmm16, %[fours]               \n"
        "vpternlogd     $0xe8, %%zmm30, %%zmm16, %%zmm14                \n"

        "vmovdqa64      %[fours], %%zmm30                               \n"
        "vpternlogd     $0x96, %%zmm18, %%zmm20, %[fours]               \n"
        "vpternlogd     $0xe8, %%zmm30, %%zmm20, %%zmm18                \n"

        "vmovdqa64      %[fours], %%zmm30                               \n"
        "vpternlogd     $0x96, %%zmm22, %%zmm24, %[fours]               \n"
        "vpternlogd     $0xe8, %%zmm30, %%zmm24, %%zmm22                \n"

        // eights
        "vmovdqa64      %[eights], %%zmm30                              \n"
        "vpternlogd     $0x96, %%zmm10, %%zmm14, %[eights]              \n"
        "vpternlogd     $0xe8, %%zmm30, %%zmm14, %%zmm10                \n"

        "vmovdqa64      %[eights], %%zmm30                              \n"
        "vpternlogd     $0x96, %%zmm18, %%zmm22, %[eights]              \n"
        "vpternlogd     $0xe8, %%zmm30, %%zmm22, %%zmm18                \n"

        // sixteens
        "vmovdqa64      %[sixteens], %[thirtytwos]                      \n"
        "vpternlogd     $0x96, %%zmm10, %%zmm18, %[sixteens]            \n"
        "vpternlogd     $0xe8, %%zmm10, %%zmm18, %[thirtytwos]          \n"

        // outputs
        : [ones]        "+x" (ones)
        , [twos]        "+x" (twos)
        , [fours]       "+x" (fours)
        , [eights]      "+x" (eights)
        , [sixteens]    "+x" (sixteens)
        , [thirtytwos]  "+x" (thirtytwos)
        , [total]       "=r" (block_total)

        // input
        : [data] "r" (data + i)
        , [tmp]  "r" (tmp)

        : "zmm10", "zmm11", "zmm12", "zmm13", "zmm14", "zmm15", "zmm16", "zmm17", "zmm18", "zmm19"
        , "zmm20", "zmm21", "zmm22", "zmm23", "zmm24", "zmm25"
        , "zmm30", "zmm31"
        , "r8", "r9", "r10", "r11", "r12", "r13", "r14", "r15"
    );

    total += block_total;
  }

  total += _mm512_popcnt(thirtytwos);
  total *= 32;
  total += 16 * _mm512_popcnt(sixteens);
  total += 8 * _mm512_popcnt(eights);
  total += 4 * _mm512_popcnt(fours);
  total += 2 * _mm512_popcnt(twos);
  total += _mm512_popcnt(ones);

  for(; i < size; i++) {
    total += _mm512_popcnt(data[i]);
  }

#undef CSA_BLOCK
#undef UPDATE_POPCNT

  return total;
}


// --- public -------------------------------------------------


uint64_t avx512f_harley_seal__hardware_popcnt_2(const uint64_t * data, size_t size) {
  const unsigned int wordspervector = sizeof(__m512i) / sizeof(uint64_t);
  const unsigned int minvit = 16 * wordspervector;
  uint64_t total;
  size_t i;

  if (size >= minvit) {
    total = popcnt_harley_seal__hardware_popcnt_2((const __m512i*) data, size / wordspervector);
    i = size - size % wordspervector;
  } else {
    total = 0;
    i = 0;
  }

  for (/**/; i < size; i++) {
    total += _mm_popcnt_u64(data[i]);
  }
  return total;
}

