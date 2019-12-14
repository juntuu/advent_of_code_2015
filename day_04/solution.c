
#include <stdio.h>
#include <string.h>

#include <CommonCrypto/CommonDigest.h>

int step(char *s, int d) {
	for (int i = d-1; i >= 0; i--) {
		if (++s[i] > '9') {
			if (i == 0) {
				s[i] = '1';
				s[d] = '0';
				return d + 1;
			}
			s[i] = '0';
		} else break;
	}
	return d;
}

int brute_force(char *str, int start, int from, unsigned char *mask, int len) {
	int n = sprintf(str+start, "%d", from);
	int res = from;
	unsigned char digest[16];
	int winner = 0;
	while (!winner) {
		CC_MD5(str, start + n, digest);
		winner = res;
		for (int i = 0; i < len; i++) {
			if (digest[i] & mask[i]) {
				winner = 0;
				break;
			}
		}
		n = step(str+start, n);
		res++;
	}
	return winner;
}

int main(int argc, char **argv) {
	char str[32] = {};
	memset(str, '0', 31);
	char *input = "yzbqklnj";
	if (argc > 1) input = argv[1];
	int n = strlen(input);
	strncpy(str, input, n);
	unsigned char mask[] = {0xff, 0xff, 0xf0};
	int one = brute_force(str, n, 0, mask, 3);
	printf("Day 4, part 1: %d\n", one);
	mask[2] = 0xff;
	int two = brute_force(str, n, one, mask, 3);
	printf("Day 4, part 2: %d\n", two);
}

