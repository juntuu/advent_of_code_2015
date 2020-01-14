
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>


typedef unsigned long T;
typedef struct List {
	T x;
	struct List *next;
} List;

typedef struct CallBack {
	void (*call)(struct CallBack*, List *);
	T *best;
	int *best_len;
} CallBack;

void for_each_combination(int sum, int *nums, int n, int taken, int *max_take, List *xs, CallBack *cb) {
	if (sum == 0) {
		cb->call(cb, xs);
		return;
	}
	if (n < 1 || sum < 0 || taken > *max_take) return;

	for_each_combination(sum, nums+1, n-1, taken, max_take, xs, cb);

	if (taken >= *max_take) return;
	List l = {*nums, xs};
	for_each_combination(sum-*nums, nums+1, n-1, taken+1, max_take, &l, cb);
}

void improve(CallBack *cb, List *xs) {
	T qe = 1;
	int len = 0;
	int old_len = *cb->best_len;
	for (; xs; xs = xs->next) {
		len++;
		qe *= xs->x;
	}
	if (len > old_len) return;

	if (len < old_len || qe < *cb->best) {
		*cb->best = qe;
		*cb->best_len = len;
	}
}

int main() {
	int total = 0;
	int x;
	int cap = 32;
	int *packages = malloc(cap * sizeof(int));
	if (!packages) {
		perror("allocating");
		return 1;
	}
	int n = 0;
	while (scanf("%d", &x) == 1) {
		if (n >= cap) {
			cap *= 2;
			packages = realloc(packages, cap * sizeof(int));
			if (!packages) {
				perror("allocating");
				return 1;
			}
		}
		total += x;
		packages[n++] = x;
	}

	if (total % 3 != 0) {
		fprintf(stderr, "the sum of packages must be divisible by 3\n");
		return 1;
	}

	T best = -1;
	int best_len = n;
	CallBack cb = {
		improve,
		&best,
		&best_len,
	};

	for_each_combination(total / 3, packages, n, 0, &best_len, NULL, &cb);
	printf("Day 24, part 1: %lu\n", best);
}

