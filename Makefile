FLAGS=-Wall -Wextra -pedantic -std=c++14 -O3

all: avx512popcnt
	make -C feedback

avx512popcnt: avx512popcnt.cpp binary.cpp
	$(CXX) $(FLAGS) avx512popcnt.cpp -o $@

binary.cpp: dump.asm py/dump2cpp.py
	python py/dump2cpp.py > binary.cpp
