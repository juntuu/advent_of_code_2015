
#include <stdio.h>
#include <stdlib.h>

int *realloc_or_die(int *p, int n) {
	p = realloc(p, n * sizeof(int));
	if (!p) {
		perror("allocating");
		exit(1);
	}
	return p;
}

int pair_in_list(int *list, int n, int x, int y) {
	for (int j = 0; j < n; j++) {
		if (list[j*2] == x && list[j*2 + 1] == y)
			return 1;
	}
	return 0;
}


int main() {
	int x = 0;
	int y = 0;
	int cap = 2048;
	int *visited = realloc_or_die(NULL, cap * 2);
	int n_visited = 0;
	int done = 0;
	while (!done) {
		switch (getchar()) {
			case '<': x--; break;
			case '>': x++; break;
			case 'v': y--; break;
			case '^': y++; break;
			case EOF: done = 1; break;
		}
		if (!pair_in_list(visited, n_visited, x, y)) {
			if (n_visited >= cap - 1) {
				cap *= 2;
				visited = realloc_or_die(visited, cap * 2);
			}
			visited[n_visited * 2] = x;
			visited[n_visited * 2 + 1] = y;
			n_visited++;
		}
	}
	free(visited);
	printf("Day 3, part 1: %d\n", n_visited);
}

