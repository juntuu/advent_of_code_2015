
#include <stdio.h>
#include <stdlib.h>

typedef struct List {
	int x;
	struct List *next;
} List;

int combinations(List *xs, int total, int res, int in_use, int *hist) {
	if (!xs || total < 0) {
		if (in_use) hist[in_use-1] += (total == 0);
		return res + (total == 0);
	}
	return combinations(xs->next, total, combinations(xs->next, total-xs->x, res, in_use+1, hist), in_use, hist);
}

int main(int argc, char **argv) {
	int eggnog = 150;
	if (argc > 1) {
		if (sscanf(argv[1], "%d", &eggnog) != 1 || eggnog < 0) {
			fprintf(stderr, "usage: %s [N]\n", argv[0]);
			return 1;
		}
	}
	List containers;
	int x;
	int n = 0;
	for (List *i = &containers, *last = NULL;; i = i->next) {
		if (scanf("%d", &x) != 1) {
			if (last) last->next = NULL;
			free(i);
			break;
		}
		i->next = malloc(sizeof(List));
		if (!i) {
			perror("allocating");
			return 1;
		}
		i->x = x;
		last = i;
		n++;
	}

	int *ns = calloc(n, sizeof(int));
	if (!ns) {
		perror("allocating");
		return 1;
	}

	int total = combinations(&containers, eggnog, 0, 0, ns);
	int least = 0;
	for (int i = 0; i < n && !least; i++)
		least = ns[i];

	printf("Day 17, part 1: %d\n", total);
	printf("Day 17, part 2: %d\n", least);
}

