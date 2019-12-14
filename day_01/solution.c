
#include <stdio.h>

int main() {
	int floor = 0;
	int basement = -1;
	int done = 0;
	while (!done) {
		switch (getchar()) {
			case '(': floor++; break;
			case ')': floor--; break;
			case EOF: done = 1; break;
		}
		if (basement < 0) {
			if (floor < 0)
				basement *= -1;
			else basement--;
		}
	}
	printf("Day 1, part 1: %d\n", floor);
	printf("Day 1, part 2: %d\n", basement);
}

