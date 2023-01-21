shell = bash

.PHONY: fuzz

fuzz:
	rm -f fuzz
	AFL_USE_ASAN=1 afl-gcc -Wall -Wextra -pedantic -g -w -O3 \
		fuzz.c $(shell find ../../src -name '*.c') -o fuzz
	# clear; afl-fuzz -i input -o output -M 1 -- ./fuzz
	# clear; afl-tmin -i output/min.txt -o output/min.txt -- ./fuzz
