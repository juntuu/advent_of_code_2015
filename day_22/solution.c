#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

typedef enum SpellKind {
	MAGIC_MISSILE,
	DRAIN,
	EFFECTS_START,
	SHIELD = EFFECTS_START,
	POISON,
	RECHARGE,
	NUM_SPELLS,
} SpellKind;

typedef struct Instant {
	int damage;
	int heal;
} Instant;

typedef struct Effect {
	int time;
	union {
		int damage;
		int armor;
		int mana;
	};
} Effect;

typedef struct Spell {
	SpellKind type;
	int cost;
	union {
		Instant instant;
		Effect effect;
	};
} Spell;

Spell all_spells[] = {
	{MAGIC_MISSILE, 53, .instant = {4, 0}},
	{DRAIN, 73, .instant = {2, 2}},
	{SHIELD, 113, .effect = {6, .armor = 7}},
	{POISON, 173, .effect = {6, .damage = 3}},
	{RECHARGE, 229, .effect = {5, .mana = 101}},
};

typedef struct Boss {
	int hp;
	int damage;
} Boss;

typedef struct Wizard {
	int hp;
	int mana;
	Spell spells[NUM_SPELLS];
} Wizard;

void p(Wizard *w, Boss *b, int spent, int line)
{
	printf("At line: %d\n", line);
	printf("  Spent: %d\n", spent);
	printf("  Wizard{%d, %d}\n", w->hp, w->mana);
	printf("  Boss{%d}\n", b->hp);
}

void print_spell(Spell *s)
{
	switch (s->type) {
		case MAGIC_MISSILE:
			printf("M{%d, %d}", s->instant.damage, s->instant.heal);
			break;
		case DRAIN:
			printf("D{%d, %d}", s->instant.damage, s->instant.heal);
			break;
		case SHIELD:
			printf("S{%d, %d}", s->effect.time, s->effect.armor);
			break;
		case POISON:
			printf("P{%d, %d}", s->effect.time, s->effect.damage);
			break;
		case RECHARGE:
			printf("R{%d, %d}", s->effect.time, s->effect.mana);
			break;
		default:
			break;
	}
}

void print(Wizard *w, int l)
{
	printf("%*d:", l+1, l);
	printf("Wizard%*.s{%3d, %3d, [ ", 16-l, "", w->hp, w->mana);
	print_spell(&w->spells[0]);
	for (int i = 1; i < NUM_SPELLS; i++) {
		printf(", ");
		print_spell(&w->spells[i]);
	}
	printf(" ] }\n");
}

int apply_effects(Wizard *w, Boss *b)
{
	if (w->spells[RECHARGE].effect.time) {
		w->mana += w->spells[RECHARGE].effect.mana;
		w->spells[RECHARGE].effect.time--;
	}
	if (w->spells[POISON].effect.time) {
		b->hp -= w->spells[POISON].effect.damage;
		w->spells[POISON].effect.time--;
	}
	if (w->spells[SHIELD].effect.time)
		w->spells[SHIELD].effect.time--;
	return b->hp < 1;
}

int boss_attack(Wizard *w, Boss *b)
{
	int hit = b->damage;
	if (w->spells[SHIELD].effect.time)
		hit -= w->spells[SHIELD].effect.armor;
	w->hp -= hit < 1 ? 1 : hit;
	return w->hp < 1;
}

int wizard_attack(Wizard *w, SpellKind spell, Boss *b)
{
	if (spell < EFFECTS_START) {
		b->hp -= w->spells[spell].instant.damage;
		w->hp += w->spells[spell].instant.heal;
	} else {
		w->spells[spell] = all_spells[spell];
	}
	return b->hp < 1;
}

int VERBOSE = 0;
int moves[1024];
int *sp = moves;

void print_moves(int spent)
{
	printf("Victory with %d spent. Moves:\n", spent);
	for (int *m = moves; m != sp; m++) {
		printf("\t");
		switch (*m) {
		case MAGIC_MISSILE:
			printf("MAGIC_MISSILE");
			break;
		case DRAIN:
			printf("DRAIN");
			break;
		case SHIELD:
			printf("SHIELD");
			break;
		case POISON:
			printf("POISON");
			break;
		case RECHARGE:
			printf("RECHARGE");
			break;
		default:
			printf("<unknown>");
			break;
		}
		printf("\n");
	}
	printf("\n");
}

int play(Wizard w, Boss b, int penalty, int spent, int limit)
{
	w.hp -= penalty;
	if (w.hp < 1)
		return 0;
	if (apply_effects(&w, &b)) {
		if (VERBOSE)
			print_moves(spent);
		return spent;
	}
	sp++;
	int best = limit;
	for (int i = 0; i < NUM_SPELLS; i++) {
		sp[-1] = i;
		int cost = w.spells[i].cost;
		if (cost > w.mana || spent + cost >= best)
			break;
		if (i >= EFFECTS_START && w.spells[i].effect.time != 0)
			continue;
		Wizard w1 = w;
		Boss b1 = b;
		w1.mana -= cost;
		if (wizard_attack(&w1, i, &b1) || apply_effects(&w1, &b1)) {
			best = spent + cost;
			if (VERBOSE)
				print_moves(best);
		} else if (!boss_attack(&w1, &b1)) {
			int res = play(w1, b1, penalty, spent + cost, best);
			if (res)
				best = res;
		}
	}
	sp--;
	return best < limit ? best : 0;
}

int main(int argc, char **argv)
{
	if (argc > 1 && strcmp(argv[1], "-v") == 0)
		VERBOSE = 1;
	Wizard player;
	player.hp = 50;
	player.mana = 500;
	for (int i = 0; i < NUM_SPELLS; i++) {
		player.spells[i] = all_spells[i];
		if (i >= EFFECTS_START)
			player.spells[i].effect.time = 0;
	}
	Boss boss;

	int ok = 1;
	ok *= scanf("Hit Points: %d\n", &boss.hp);
	ok *= scanf("Damage: %d\n", &boss.damage);
	if (ok != 1) {
		fprintf(stderr, "bad input\n");
		return 1;
	}

	printf("Day 22, part 1: %d\n", play(player, boss, 0, 0, INT_MAX));
	printf("Day 22, part 2: %d\n", play(player, boss, 1, 0, INT_MAX));
}
