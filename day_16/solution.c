
#include <stdio.h>

enum {
	children,
	cats,
	samoyeds,
	pomeranians,
	akitas,
	vizlas,
	goldfish,
	trees,
	cars,
	perfumes,
	N,
};

/*
Sue 1: children: 1, cars: 8, vizslas: 7
Sue 2: akitas: 10, perfumes: 10, children: 5
*/
int parse_aunt(FILE *f, int *a) {
	int x;
	char c[3];
	fscanf(f, "%*s %*d:");
	do {
		if (2 != fscanf(f, " %3c%*s %d", c, &x))
			return -1;
		switch (c[0]) {
			case 'a': a[akitas] = x; break;
			case 'g': a[goldfish] = x; break;
			case 's': a[samoyeds] = x; break;
			case 't': a[trees] = x; break;
			case 'v': a[vizlas] = x; break;
			case 'c': switch (c[2]) {
				case 'r': a[cars] = x; break;
				case 't': a[cats] = x; break;
				case 'i': a[children] = x; break;
				} break;
			case 'p': switch (c[2]) {
				case 'r': a[perfumes] = x; break;
				case 'm': a[pomeranians] = x; break;
				} break;
		}
	} while (',' == getc(f));
	return 0;
}

int match(int *m, int *a) {
	for (int n = 0; n < N; n++) {
		if (a[n] >= 0 && a[n] != m[n])
			return 0;
	}
	return 1;
}

int match2(int *m, int *a) {
	for (int n = 0; n < N; n++) {
		if (a[n] < 0) continue;
		switch (n) {
			case cats:
			case trees:
				if (a[n] <= m[n])
					return 0;
				break;
			case goldfish:
			case pomeranians:
				if (a[n] >= m[n])
					return 0;
				break;
			default:
				if (a[n] != m[n])
					return 0;
				break;
		}
	}
	return 1;
}

/*
 * children: 3
 * cats: 7
 * samoyeds: 2
 * pomeranians: 3
 * akitas: 0
 * vizslas: 0
 * goldfish: 5
 * trees: 3
 * cars: 2
 * perfumes: 1
 */

int main() {
	int sample[] = {3, 7, 2, 3, 0, 0, 5, 3, 2, 1};
	int sue[N];
	int matching[2] = {};
	for (int i = 1;; i++) {
		for (int n = 0; n < N; n++)
			sue[n] = -1;
		if (parse_aunt(stdin, sue))
			break;
		if (match(sample, sue)) {
			matching[0] = i;
			if (matching[1])
				break;
		}
		if (match2(sample, sue)) {
			matching[1] = i;
			if (matching[0])
				break;
		}
	}
	printf("Day 16, part 1: %d\n", matching[0]);
	printf("Day 16, part 2: %d\n", matching[1]);
}

