# minimalist makefile
.SUFFIXES:
.PHONY: all clean

.SUFFIXES: .cpp .o .c .h
CFLAGS = -fPIC -std=gnu99 -Wall -Wextra -Wshadow -mavx512f -march=native

all: basic_benchmark


HEADERS=tabulated_hamming_weight.h \
        avx512f_hamming_weight.h

OBJECTS=tabulated_hamming_weight.o \
		avx512f_hamming_weight.o

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $<

basic_benchmark: basic_benchmark.c benchmark.h $(HEADERS) $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ basic_benchmark.c $(OBJECTS)

clean:
	rm -f basic_benchmark *.o
