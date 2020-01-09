
#include <stdio.h>
#include <stdlib.h>

enum {
	HLF = 'h',
	TPL = 't',
	INC = 'i',
	JMP = 'p',
	JIE = 'e',
	JIO = 'o',
};

typedef struct {
	int i;
	int r;
	int v;
} Instruction;

typedef unsigned T;
typedef struct {
	T a;
	T b;
} Registers;

void run(Instruction *i, int n, Registers *reg) {
	Instruction *start = i;
	Instruction *end = i+n;
	while (start <= i && i < end) {
		T *r = i->r ? &reg->b : &reg->a;
		switch (i->i) {
			case HLF:
				*r /= 2;
				i++;
				break;
			case TPL:
				*r *= 3;
				i++;
				break;
			case INC:
				(*r)++;
				i++;
				break;
			case JMP:
				i += i->v;
				break;
			case JIE:
				if (*r % 2 == 0) i += i->v;
				else i++;
				break;
			case JIO:
				if (*r == 1) i += i->v;
				else i++;
				break;
			default: return;
		}
	}
}

int main() {
	int cap = 64;
	Instruction *instr = malloc(cap * sizeof(Instruction));
	if (!instr) {
		perror("allocating");
		return 1;
	}
	int n = 0;
	while (1) {
		if (n >= cap) {
			cap *= 2;
			instr = realloc(instr, cap * sizeof(Instruction));
			if (!instr) {
				perror("allocating");
				return 1;
			}
		}
		Instruction i = {};
		int c = getchar();
		if (c == EOF) break;
		switch (c) {
			case 'h':
			case 't':
			case 'i':
				i.i = c;
				scanf("%*3c");
				i.r = getchar() == 'b';
				instr[n++] = i;
				break;
			case 'j':
				getchar();
				i.i = getchar();
				getchar();
				if (i.i != 'p') {
					i.r = getchar() == 'b';
					getchar();
				}
				scanf("%d", &i.v);
				instr[n++] = i;
				break;
		}
	}

	Registers reg = {0, 0};
	run(instr, n, &reg);
	printf("Day 23, part 1: %u\n", reg.b);

	reg.a = 1;
	reg.b = 0;
	run(instr, n, &reg);
	printf("Day 23, part 1: %u\n", reg.b);

	free(instr);
}

