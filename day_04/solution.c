
#include <stdio.h>
#include <string.h>

#include <CommonCrypto/CommonDigest.h>

int brute_force(char *str, int start) {
	int n = 1;
	int res = 0;
	unsigned char digest[16];
	while (1) {
		n = sprintf(str+start, "%d", res);
		str[start+n] = '\0';
		CC_MD5(str, start + n, digest);
		if (!(digest[0] || digest[1] || (digest[2] & 0xf0)))
			return res;
		res++;
	}
	return -1;
}

int main(int argc, char **argv) {
	char str[32] = {};
	char *input = "yzbqklnj";
	if (argc > 1) input = argv[1];
	int n = strlen(input);
	strncpy(str, input, n);
	printf("Day 4, part 1: %d\n", brute_force(str, n));
}

