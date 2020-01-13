
#include <stdio.h>
#include <stdlib.h>

typedef struct {
	char *name;
	int hp;
	int damage;
	int armor;
} Player;

typedef struct {
	int cost;
	int damage;
	int armor;
} Item;

int attack(Player *attacker, Player *defender) {
	int hit = attacker->damage - defender->armor;
	defender->hp -= hit > 0 ? hit : 1;
	return defender->hp <= 0;
}

int play(Player a, Player b) {
	while (1) {
		if (attack(&a, &b)) return 0;
		if (attack(&b, &a)) return 1;
	}
}

int equip_with(Player *p, Item *i) {
	p->damage += i->damage;
	p->armor += i->armor;
	return i->cost;
}

typedef struct List {
	int x;
	struct List *next;
} List;

typedef struct CallBack {
	void (*call)(struct CallBack*, void *);
	Player *p;
	Player *e;
	int *best;
} CallBack;

void for_each_combination(int n, List *total, List *xs, CallBack *cb) {
	if (n < 1) cb->call(cb, xs);
	else for (int i = 0; i <= total->x; i++) {
		List l = {i, xs};
		for_each_combination(n-1, total->next, &l, cb);
	}
}

enum {
	WEAPON,
	ARMOR,
	RING,
};
struct {int n; Item items[6];} shop[] = {
	{5, {{8, 4, 0}, {10, 5, 0}, {25, 6, 0}, {40, 7, 0}, {74, 8, 0}}},
	{5, {{13, 0, 1}, {31, 0, 2}, {53, 0, 3}, {75, 0, 4}, {102, 0, 5}}},
	{6, {{20, 0, 1}, {25, 1, 0}, {40, 0, 2}, {50, 2, 0}, {80, 0, 3}, {100, 3, 0}}}
};

void try_play(CallBack *c, void *l) {
	List *items = l;
	Player p = *c->p;
	Item *i = shop[WEAPON].items;
	int gold = equip_with(&p, &i[items->x]);
	items = items->next;
	i = shop[ARMOR].items;
	if (items->x)
		gold += equip_with(&p, &i[items->x - 1]);
	items = items->next;
	i = shop[RING].items;
	if (items->x)
		gold += equip_with(&p, &i[items->x - 1]);
	if (items->next->x && items->next->x != items->x)
		gold += equip_with(&p, &i[items->next->x - 1]);

	if (play(p, *c->e) == 0 && gold < *c->best)
		*c->best = gold;
}

int main() {
	Player player = {"player", 100, 0, 0};
	Player boss = {.name = "boss"};

	int ok = 1;
	ok *= scanf("Hit Points: %d\n", &boss.hp);
	ok *= scanf("Damage: %d\n", &boss.damage);
	ok *= scanf("Armor: %d\n", &boss.armor);
	if (ok != 1) {
		fprintf(stderr, "bad input\n");
		return 1;
	}

	int min_gold = 1000;
	CallBack cb = {
		try_play,
		&player,
		&boss,
		&min_gold,
	};
	List totals[] = {
		{6, &totals[1]},
		{6, &totals[2]},
		{5, &totals[3]},
		{4, NULL},
	};
	for_each_combination(4, totals, NULL, &cb);

	printf("Day 21, part 1: %d\n", min_gold);
}

