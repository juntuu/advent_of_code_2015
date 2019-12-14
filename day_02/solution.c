
#include <stdio.h>

int min(int a, int b) {
	return b < a ? b : a;
}

int paper_for_box(int w, int h, int l) {
	int a = w * h;
	int b = l * h;
	int c = l * w;
	return 2 * (a + b + c) + min(a, min(b, c));
}

int main() {
	int paper = 0;
	int x, y, z;
	while (scanf("%dx%dx%d", &x, &y, &z) == 3)
		paper += paper_for_box(x, y, z);
	printf("Day 2, part 1: %d\n", paper);
}

