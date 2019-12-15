
#include <stdio.h>

/*
 * A nice string is one with all of the following properties:
 *
 *     It contains at least three vowels (aeiou only), like aei, xazegov, or
 *     aeiouaeiouaeiou.  It contains at least one letter that appears twice in
 *     a row, like xx, abcdde (dd), or aabbccdd (aa, bb, cc, or dd).  It does
 *     not contain the strings ab, cd, pq, or xy, even if they are part of one
 *     of the other requirements.
 */

int is_nice(char *s) {
	int vowels = 0;
	int seq = 0;
	for (int i = 0;; i++) {
		switch (s[i]) {
			case '\n':
			case '\0': return seq && vowels >= 3;
			case 'a':
			case 'e':
			case 'i':
			case 'o':
			case 'u': vowels++; break;
			case 'b': if (i > 0 && s[i-1] == 'a') return 0; break;
			case 'd': if (i > 0 && s[i-1] == 'c') return 0; break;
			case 'q': if (i > 0 && s[i-1] == 'p') return 0; break;
			case 'y': if (i > 0 && s[i-1] == 'x') return 0; break;
		}
		if (!seq && i > 0 && s[i-1] == s[i]) seq = 1;
	}
}

int main() {
	int nice = 0;
	char buf[256];
	while (fgets(buf, sizeof(buf), stdin))
		nice += is_nice(buf);
	printf("Day 5, part 1: %d\n", nice);
}

