
#include <stdio.h>
#include <stdlib.h>

enum {
	TOGGLE = '\0',
	OFF = 'f',
	ON = 'n',
	PLUS2 = '\0' + 1,
	DECR = 'f' + 1,
	INCR = 'n' + 1,
	SIZE = 1000,
} Grid;

void set_lights(char *grid, int mode, int x0, int y0, int x1, int y1) {
	for (int j = y0; j <= y1; j++) {
		int base = j * SIZE;
		for (int i = x0; i <= x1; i++) {
			switch (mode) {
				case TOGGLE:
					grid[base + i] ^= 1;
					break;
				case OFF:
					grid[base + i] = 0;
					break;
				case ON:
					grid[base + i] = 1;
					break;
				case PLUS2:
					grid[base + i] += 2;
					break;
				case DECR:
					if (grid[base + i])
						grid[base + i]--;
					break;
				case INCR:
					grid[base + i]++;
					break;
			}
		}
	}
}

int main() {
	char *grid = calloc(SIZE * SIZE, sizeof(char));
	char *grid2 = calloc(SIZE * SIZE, sizeof(char));
	if (!grid || !grid2) {
		perror("allocating");
		return 1;
	}
	int x0, y0, x1, y1;
	char c[4];
	while (5 == scanf("%*5c%3s%d,%d %*s %d,%d\n", c, &x0, &y0, &x1, &y1)) {
		set_lights(grid, c[1], x0, y0, x1, y1);
		set_lights(grid2, c[1]+1, x0, y0, x1, y1);
	}
	int lit = 0;
	int brightness = 0;
	for (int i = 0; i < SIZE * SIZE; i++) {
		lit += grid[i];
		brightness += grid2[i];
	}
	printf("Day 6, part 1: %d\n", lit);
	printf("Day 6, part 2: %d\n", brightness);
	free(grid);
	free(grid2);
}

