
#include <stdio.h>
#include <stdlib.h>

typedef struct List {
	int x;
	struct List *next;
} List;

int combinations(List *xs, int total, int res) {
	if (!xs || total < 0)
		return res + (total == 0);
	return combinations(xs->next, total, combinations(xs->next, total-xs->x, res));
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
	}

	int n = combinations(&containers, eggnog, 0);
	printf("Day 17, part 1: %d\n", n);
}

