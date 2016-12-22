#define _POSIX_C_SOURCE 200809L
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <string.h>
#include "benchmark.h"
#include "hamming_weight.h"

void *aligned_malloc(size_t alignment, size_t size) {
  void *mem;
  if (posix_memalign(&mem, alignment, size))
    exit(1);
  return mem;
}

void demo(int size) {
  printf("size = %d words or %lu bytes \n", size, size * sizeof(uint64_t));
  int repeat = 500;
  uint64_t *prec = aligned_malloc(64, size * sizeof(uint64_t));
  for (int k = 0; k < size; ++k)
    prec[k] = -k;

  int expected = table_bitset8_weight((uint8_t*)prec, size * 8);

  BEST_TIME(avx512f_harley_seal__hardware_popcnt_2(prec, size), expected, ,
            repeat, size);

  free(prec);
  printf("\n");
}

int main() {

  demo(512);
  demo(1024);
  demo(4096);
  demo(8192);

  return 0;
}
