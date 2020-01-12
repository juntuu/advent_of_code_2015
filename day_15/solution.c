
#include <stdio.h>
#include <stdlib.h>

typedef struct {
	int capacity;
	int durability;
	int flavor;
	int texture;
	int calories;
} Ingredient;

typedef struct List {
	union {
		void *data;
		int x;
	};
	struct List *next;
} List;

typedef struct CallBack {
	void (*call)(struct CallBack*, void *);
	List *ingredients;
	int *best;
	int calories;
	int *best_with_calories;
} CallBack;

int read_ingredient(FILE *f, Ingredient *i) {
	return 5 == fscanf(
			f,
			"%*s %*s %d, %*s %d, %*s %d, %*s %d, %*s %d",
			&i->capacity,
			&i->durability,
			&i->flavor,
			&i->texture,
			&i->calories);
}

typedef struct { int score; int calories; } Score;
Score score(List *ingredients, List *k) {
	Score s = {1, 0};
	int total[4] = {0, 0, 0, 0};
	while (k) {
		Ingredient *i = ingredients->data;
		total[0] += k->x * i->capacity;
		total[1] += k->x * i->durability;
		total[2] += k->x * i->flavor;
		total[3] += k->x * i->texture;
		s.calories += k->x * i->calories;
		k = k->next;
		ingredients = ingredients->next;
	}

	for (int i = 0; i < 4; i++) {
		if (total[i] < 0) {
			s.score = 0;
			break;
		}
		s.score *= total[i];
	}
	return s;
}

void for_each_combination(int n, int total, List *xs, CallBack *cb) {
	if (n == 1) {
		List l = {{.x = total}, xs};
		cb->call(cb, &l);
		return;
	}
	for (int i = 0; i <= total; i++) {
		List l = {{.x = i}, xs};
		for_each_combination(n-1, total-i, &l, cb);
	}
}

void improve_score(CallBack *cb, void *k) {
	Score s = score(cb->ingredients, k);
	if (s.score > *cb->best)
		*cb->best = s.score;
	if (s.score > *cb->best_with_calories && s.calories == cb->calories)
		*cb->best_with_calories = s.score;
}

int main() {
	List ingredients;
	int n = 0;
	for (List *i = &ingredients;; i = i->next) {
		i->next = malloc(sizeof(List));
		i->data = malloc(sizeof(Ingredient));
		if (!i->next || !i->x) {
			fprintf(stderr, "couldn't allocate\n");
			return 1;
		}
		if (!read_ingredient(stdin, i->data))
			break;
		n++;
	}

	int best = 0;
	int best_with_calories = 0;
	CallBack cb = {
		improve_score,
		&ingredients,
		&best,
		500,
		&best_with_calories,
	};

	for_each_combination(n, 100, NULL, &cb);

	printf("Day 15, part 1: %d\n", best);
	printf("Day 15, part 2: %d\n", best_with_calories);
}

