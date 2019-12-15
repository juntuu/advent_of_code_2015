
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

/*
 * Now, a nice string is one with all of the following properties:
 *
 *     It contains a pair of any two letters that appears at least twice in the
 *     string without overlapping, like xyxy (xy) or aabcdefgaa (aa), but not
 *     like aaa (aa, but it overlaps).  It contains at least one letter which
 *     repeats with exactly one letter between them, like xyx, abcdefeghi
 *     (efe), or even aaa.
 */

int check_pair(char *pair, char *rest) {
	for (int i = 0; rest[i] != '\0' && rest[i] != '\n'; i++) {
		if (pair[1] == rest[i] && pair[0] == rest[i-1])
			return 1;
	}
	return 0;
}

int is_nice2(char *s) {
	int repeat = 0;
	int pair = 0;
	for (int i = 0; s[i] != '\0' && s[i] != '\n'; i++) {
		if (!repeat && i > 1 && s[i-2] == s[i]) repeat = 1;
		if (!pair && i > 2 && check_pair(&s[i-3], &s[i])) pair = 1;
		if (repeat && pair) return 1;
	}
	return 0;
}

int main() {
	int nice = 0;
	int nice2 = 0;
	char buf[256];
	while (fgets(buf, sizeof(buf), stdin)) {
		nice += is_nice(buf);
		nice2 += is_nice2(buf);
	}
	printf("Day 5, part 1: %d\n", nice);
	printf("Day 5, part 2: %d\n", nice2);
}

