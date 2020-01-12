
#include <stdio.h>
#include <stdlib.h>

void swap(int *a, int *b) {
	int tmp = *a;
	*a = *b;
	*b = tmp;
}

int first_permutation(int *array, int n) {
	if (!array) return 0;
	while (n--) array[n] = n;
	return 1;
}

int next_permutation(int *array, int n) {
	int i, j;
	i = j = n - 1;

	while (i > 0 && array[i - 1] >= array[i]) i--;
	if (i < 1) return 0;

	while (array[j] <= array[i - 1]) j--;
	swap(&array[i-1], &array[j]);

	j = n - 1;
	while (i < j) swap(&array[i++], &array[j--]);
	return 1;
}

int seating_score(int *scores, int *order, int size) {
	int total = 0;
	for (int i = 0; i < size; i++) {
		total += scores[order[i] * size + order[(i+1)%size]];
		total += scores[order[(i+1)%size] * size + order[i]];
	}
	return total;
}

#define N_PERSONS 8
int main(int argc, char **argv) {
	int n = N_PERSONS;
	if (argc > 1) {
		if (sscanf(argv[1], "%d", &n) != 1) {
			fprintf(stderr, "bad number: %s\n", argv[1]);
			fprintf(stderr, "usage: %s [NUMBER]\n", argv[0]);
			return 1;
		}
		if (n < 0) {
			fprintf(stderr, "number must be positive, got %d\n", n);
			return 1;
		}
	}
	int *scores = calloc(n * n, sizeof(int));
	int *order = malloc(n * sizeof(int));
	if (!scores || !order) {
		fprintf(stderr, "couldn't allocate\n");
		return 1;
	}

	for (int i = 0; i < n; i++) {
		int d;
		char c;
		for (int j = 0; j < n; j++) {
			if (j == i) continue;
			if (scanf("%*s%*s %c%*s %d %*[^.].", &c, &d) != 2) {
				fprintf(stderr, "bad input\n");
				return 1;
			}
			if (c == 'l') d *= -1;
			scores[i * n + j] = d;
		}
	}

	first_permutation(order, n);

	int score = seating_score(scores, order, n);

	while (next_permutation(order, n)) {
		int d = seating_score(scores, order, n);
		if (d > score) score = d;
	}

	printf("Day 13, part 1: %d\n", score);

	free(scores);
	free(order);
}

