
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

T next_code(T x) {
	return 252533 * x % 33554393;
}

T code_rec(Index i) {
	static T table[6][6] = {
		{ 20151125, 18749137, 17289845, 30943339, 10071777, 33511524 },
		{ 31916031, 21629792, 16929656,  7726640, 15514188,  4041754 },
		{ 16080970,  8057251,  1601130,  7981243, 11661866, 16474243 },
		{ 24592653, 32451966, 21345942,  9380097, 10600672, 31527494 },
		{    77061, 17552253, 28094349,  6899651,  9250759, 31663883 },
		{ 33071741,  6796745, 25397450, 24659492,  1534922, 27995004 },
	};
	if (i.col < 7 && i.row < 7) {
		return table[i.row-1][i.col-1];
	}

	return next_code(code_rec(prev(i)));
}

T code(Index i) {
	if (i.col < 1 || i.row < 1) {
		fprintf(stderr, "bad index: {%d, %d}\n", i.row, i.col);
		return 0;
	}
	Index base = {6, 6};
	int n = numeric_index(base);
	int max = numeric_index(i);
	if (max <= n) return code_rec(i);
	T res = code_rec(base);
	while (n++ < max) res = next_code(res);
	return res;
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

