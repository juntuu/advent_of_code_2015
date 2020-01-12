
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

int journey_distance(int *distances, int *order, int size) {
	int total = 0;
	for (int i = 0; i < size-1; i++)
		total += distances[order[i] * size + order[i+1]];
	return total;
}

#define N_CITIES 8
int main(int argc, char **argv) {
	int n = N_CITIES;
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
	int *distances = calloc(n * n, sizeof(int));
	int *order = malloc(n * sizeof(int));
	if (!distances || !order) {
		fprintf(stderr, "couldn't allocate\n");
		return 1;
	}

	for (int i = 0; i < n; i++) {
		for (int j = i+1; j < n; j++) {
			if (scanf("%*[^=]= %d", &distances[i * n + j]) != 1) {
				fprintf(stderr, "bad input\n");
				return 1;
			}
			distances[j * n + i] = distances[i * n + j];
		}
	}

	first_permutation(order, n);

	int shortest = journey_distance(distances, order, n);
	int longest = shortest;

	while (next_permutation(order, n)) {
		int d = journey_distance(distances, order, n);
		if (d < shortest) shortest = d;
		if (d > longest) longest = d;
	}

	printf("Day 9, part 1: %d\n", shortest);
	printf("Day 9, part 2: %d\n", longest);

	free(distances);
	free(order);
}

