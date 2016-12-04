FLAGS=-Wall -Wextra -pedantic -std=c++14 -O3

avx512popcnt: avx512popcnt.cpp binary.cpp
	$(CXX) $(FLAGS) avx512popcnt.cpp -o avx512popcnt

binary.cpp: dump.asm dump2cpp.py
	python dump2cpp.py > binary.cpp
