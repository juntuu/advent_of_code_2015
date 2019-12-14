
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
	int x[] = {0, 0, 0};
	int y[] = {0, 0, 0};
	int cap_santa = 2048;
	int cap_both = 2048;
	int *santa = realloc_or_die(NULL, cap_santa * 2);
	int *both = realloc_or_die(NULL, cap_both * 2);
	int n_santa = 0;
	int n_both = 0;
	int done = 0;
	int i = 0;
	while (!done) {
		switch (getchar()) {
			case '<': x[2]--; x[i%2]--; break;
			case '>': x[2]++; x[i%2]++; break;
			case 'v': y[2]--; y[i%2]--; break;
			case '^': y[2]++; y[i%2]++; break;
			case EOF: done = 1; break;
		}
		if (!pair_in_list(santa, n_santa, x[2], y[2])) {
			if (n_santa >= cap_santa - 1) {
				cap_santa *= 2;
				santa = realloc_or_die(santa, cap_santa * 2);
			}
			santa[n_santa * 2] = x[2];
			santa[n_santa * 2 + 1] = y[2];
			n_santa++;
		}
		if (!pair_in_list(both, n_both, x[i%2], y[i%2])) {
			if (n_both >= cap_both - 1) {
				cap_both *= 2;
				both = realloc_or_die(both, cap_both * 2);
			}
			both[n_both * 2] = x[i%2];
			both[n_both * 2 + 1] = y[i%2];
			n_both++;
		}
		i++;
	}
	free(santa);
	free(both);
	printf("Day 3, part 1: %d\n", n_santa);
	printf("Day 3, part 2: %d\n", n_both);
}

