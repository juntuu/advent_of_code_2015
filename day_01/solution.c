
#include <stdio.h>

int main() {
	int floor = 0;
	int done = 0;
	while (!done) {
		switch (getchar()) {
			case '(': floor++; break;
			case ')': floor--; break;
			case EOF: done = 1; break;
		}
	}
	printf("Day 1, part 1: %d\n", floor);
}

