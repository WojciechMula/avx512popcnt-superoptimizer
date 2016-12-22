#ifndef _AVX512F_HAMMING_WEIGHT_
#define _AVX512F_HAMMING_WEIGHT_

// AVX512F version of Harley-Seal, using only "foundation" instruction + popcnt instruction
// changed order of computations
uint64_t avx512f_harley_seal__hardware_popcnt_2(const uint64_t * data, size_t size);

#endif // _AVX512F_HAMMING_WEIGHT_
