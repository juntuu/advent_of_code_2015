
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum {
	DONE = 0,
	MASK = ~0x300,
	A = 0x100,
	B = 0x200,
	OR = 'O',
	AND = 'A',
	NOT = 'N',
	RSHIFT = 'R',
	LSHIFT = 'L',
} Gate;

typedef struct {
	uint16_t id;
	uint16_t gate;
	uint16_t a;
	uint16_t b;
} Wire;

#define N 27
uint16_t hash(char *s) {
	int a = *s - 'a' + 1;
	int b = *(s+1) - 'a' + 1;
	if (b < 0 || b >= N) b = 0;
	return N * a + b;
}

int parse_num(FILE *f, int start) {
	start = start - '0';
	int c;
	while((c = getc(f)) <= '9' && c >= '0')
		start = start * 10 + c - '0';
	return start;
}

int parse(FILE *f, Wire *wires) {
	int n = 0;
	int c = 0;
	int i = 0;
	int val = 0;
	Wire w = {};
	while ((c = getc(f)) != EOF) {
		switch (c) {
			case '>':
			case '-':
			case ' ':
				break;
			case '\n':
store:
				i = 0;
				n++;
				w.id = val;
				wires[val] = w;
				w = (Wire){};
				break;
			case 'R':
			case 'L':
			case 'O':
			case 'A':
			case 'N':
				w.gate |= c;
				while ((c = getc(f)) != ' ');
				break;
			default:
				i++;
				if ('0' <= c && c <= '9')
					val = parse_num(f, c);
				else {
					if (i == 1) w.gate |= A;
					else if (i == 2) w.gate |= B;
					char id[] = {c, getc(f)};
					val = hash(id);
					if (id[1] == '\n') goto store;
				}
				if (i == 1) w.a = val;
				else if (i == 2) w.b = val;
				break;
		}
	}
	return n;
}

uint16_t solve(int i, Wire *ws) {
	Wire *w = &ws[i];
	int g = w->gate;
	w->gate &= ~A;
	w->gate &= ~B;
	if (g & A) w->a = solve(w->a, ws);
	if (g & B) w->b = solve(w->b, ws);
	switch (g & MASK) {
		case NOT:
			return ~w->a;
		case OR:
			return w->a | w->b;
		case AND:
			return w->a & w->b;
		case LSHIFT:
			return w->a << w->b;
		case RSHIFT:
			return w->a >> w->b;
		default:
			return w->a;
	}
}

int main(int argc, char **argv) {
	FILE *f = stdin;
	if (argc > 1) {
		f = fopen(argv[1], "r");
		if (!f) {
			perror(argv[1]);
			return 1;
		}
	}
	Wire *wires = calloc(N * N, sizeof(Wire));
	Wire *copy = malloc(N * N * sizeof(Wire));
	if (!wires || !copy) {
		perror("allocating");
		fclose(f);
		return 1;
	}
	parse(f, wires);
	fclose(f);

	memcpy(copy, wires, N * N * sizeof(Wire));

	uint16_t res = solve(hash("a"), wires);
	printf("Day 7, part 1: %d\n", res);
	free(wires);

	Wire *b = &copy[hash("b")];
	b->gate = DONE;
	b->a = res;

	res = solve(hash("a"), copy);
	printf("Day 7, part 2: %d\n", res);
	free(copy);
}

