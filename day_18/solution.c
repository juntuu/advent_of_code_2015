
#include <stdio.h>
#include <unistd.h>

#ifndef N
#define N 100
#endif

typedef char Grid[N][N + 1];

int around(Grid *g, int x, int y) {
	int on = 0;
	for (int j = y-1; j <= y+1; j++) {
		if (j < 0 || j >= N) continue;
		for (int i = x-1; i <= x+1; i++) {
			if (i < 0 || i >= N) continue;
			if ((*g)[j][i] == '#') on++;
		}
	}
	return on;
}

/*
 * A light which is on stays on when 2 or 3 neighbors are on, and turns off
 * otherwise.
 *
 * A light which is off turns on if exactly 3 neighbors are on, and stays off
 * otherwise.
 */

void update(Grid *old, Grid *new) {
	for (int j = 0; j < N; j++) {
		for (int i = 0; i < N; i++) {
			int on = around(old, i, j);
			(*new)[j][i] =
				(on == 3 || ((*old)[j][i] == '#' && on == 4))
				? '#'
				: '.';
		}
	}
}

void update2(Grid *old, Grid *new) {
	(*old)[0][0] = '#';
	(*old)[0][N-1] = '#';
	(*old)[N-1][0] = '#';
	(*old)[N-1][N-1] = '#';
	update(old, new);
	(*new)[0][0] = '#';
	(*new)[0][N-1] = '#';
	(*new)[N-1][0] = '#';
	(*new)[N-1][N-1] = '#';
}

void print_grid(Grid *g) {
	for (int y = 0; y < N; y++) {
		puts((*g)[y]);
	}
}

int lights_on(Grid *g) {
	int on = 0;
	for (int y = 0; y < N; y++) {
		for (int x = 0; x < N; x++) {
			on += (*g)[y][x] == '#';
		}
	}
	return on;
}

int main() {
	Grid grid = {};
	Grid swap = {};
	Grid grid2 = {};
	Grid swap2 = {};
	for (int i = 0; i < N; i++) {
		if (read(STDIN_FILENO, grid[i], N+1) < N+(i < N-1)) {
			fprintf(stderr, "error reading input at line %d\n", i+1);
			return 1;
		}
		grid[i][N] = '\0';
	}
	int step = 0;
	Grid *current = &grid;
	Grid *next = &swap;
	Grid *current2 = &grid2;
	Grid *next2 = &swap2;
	update(current, next);
	update2(current, next2);
	for (; step < 100-1; step++) {
		Grid *temp = current;
		current = next;
		next = temp;
		temp = current2;
		current2 = next2;
		next2 = temp;
		update(current, next);
		update2(current2, next2);
	}
	printf("Day 18, part 1: %d\n", lights_on(next));
	printf("Day 18, part 2: %d\n", lights_on(next2));
}

