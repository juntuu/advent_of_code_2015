
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void die(char *msg) {
	if (msg) fprintf(stderr, "error: %s\n", msg);
	else fprintf(stderr, "error\n");
	exit(1);
}

void *alloc_or_die(void *p, int n, int size) {
	if (!p) p = calloc(n, size);
	else p = realloc(p, n * size);

	if (!p) die("allocating");
	return p;
}

typedef struct List {
	char *s;
	int len;
	struct List *next;
} List;

typedef struct {
	char *from;
	List *to;
} Replacement;

#define NELEM 26 * 2
typedef struct Trie {
	int word;
	struct Trie *next[NELEM];
} Trie;

int c_to_i(int c) {
	return c > 'Z' ? 26 + c - 'a' : c - 'A';
}

int insert(List *w, Trie *t) {
	if (!w) {
		if (t->word) return 0;
		t->word = 1;
		return 1;
	}

	if (w->len < 1) return insert(w->next, t);

	int i = c_to_i(w->s[0]);
	if (!t->next[i])
		t->next[i] = alloc_or_die(NULL, 1, sizeof(Trie));
	w->len--;
	w->s++;
	return insert(w, t->next[i]);
}

int replacements(char *m, int n, Replacement *r, int rn, Trie *t) {
	int uniq = 0;
	for (; rn--; r++) {
		int ok = strlen(r->from);
		for (int i = 0; i < n; i++) {
			if (strncmp(r->from, m+i, ok)) continue;
			for (List *to = r->to; to; to = to->next) {
				List w[3] = {
					{m, i, &w[1]},
					{to->s, to->len, &w[2]},
					{m+i+ok, n-i-ok, NULL},
				};
				uniq += insert(w, t);
			}
		}
	}
	return uniq;
}

int main() {
	int cap = 32;
	int n = 0;
	Replacement *rs = alloc_or_die(NULL, cap, sizeof(Replacement));
	char from[4];
	char to[32];
	char *last = NULL;
	Replacement *cur = rs;
	while (scanf("%3s => %31s\n", from, to) == 2) {
		if (!last) {
			cur->from = alloc_or_die(NULL, strlen(from)+1, sizeof(char));
			strcpy(cur->from, from);
			n++;
		} else if (strcmp(last, from) != 0) {
			cur++;
			cur->from = alloc_or_die(NULL, strlen(from)+1, sizeof(char));
			strcpy(cur->from, from);
			n++;
		}
		List *new = alloc_or_die(NULL, 1, sizeof(List));
		new->len = strlen(to);
		new->s = alloc_or_die(NULL, new->len+1, sizeof(char));
		strcpy(new->s, to);
		last = cur->from;
		new->next = cur->to;
		cur->to = new;
	}
	if (cap > n && rs[n].from) n++;

	cap = 256;
	char *molecule = alloc_or_die(NULL, cap+3, sizeof(char));
	strcpy(molecule, from);
	for (int i = 3; scanf("%255s", molecule+i) == 1; i += 255) {
		cap += 255;
		molecule = alloc_or_die(molecule, cap, sizeof(char));
	}
	int len = strlen(molecule);
	Trie *t = alloc_or_die(NULL, 1, sizeof(Trie));
	int count = replacements(molecule, len, rs, n, t);

	printf("Day 19, part 1: %d\n", count);
}

