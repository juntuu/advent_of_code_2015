#include <stdio.h>
#include <stdlib.h>

typedef unsigned long T;

typedef struct {
	int row;
	int col;
} Index;

Index prev(Index i) {
	i.col--;
	if (i.col < 1) {
		i.col = i.row - 1;
		i.row = 1;
	} else i.row++;
	return i;
};

int numeric_index(Index i) {
	T n = 0;
	while (i.col > 0 && i.row > 0) {
		i = prev(i);
		n++;
	}
	return n - 1;
}

T mod_pow(T base, T exponent, T modulus) {
	if (modulus == 1)
		return 0;
	T result = 1;
	base %= modulus;
	while (exponent) {
		if (exponent & 1)
			result = (result * base) % modulus;
		exponent >>= 1;
		base = (base * base) % modulus;
	}
	return result;
}

T code(Index i) {
	if (i.col < 1 || i.row < 1) {
		fprintf(stderr, "bad index: {%d, %d}\n", i.row, i.col);
		return 0;
	}
	const T first = 20151125;
	const T modulus = 33554393;
	const T multiplier = 252533;
	return (first * mod_pow(multiplier, numeric_index(i), modulus)) % modulus;
}

int main(int argc, char **argv) {
	Index i = {0, 0};
	if (argc == 3) {
		sscanf(argv[1], "%d", &i.row);
		sscanf(argv[2], "%d", &i.col);
	} else {
		scanf("%*[^0-9]%d", &i.row);
		scanf("%*[^0-9]%d", &i.col);
	}
	printf("Day 25, part 1: %lu\n", code(i));
}

