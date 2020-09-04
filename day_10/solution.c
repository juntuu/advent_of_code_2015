
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int rle(char *in, char *out) {
	char n = 0;
	char p = *in;
	int i = 0;
	char *c = in;
	while (*c++) {
		n++;
		if (!*c || *c != p) {
			out[i++] = '0' + n;
			out[i++] = p;
			p = *c;
			n = 0;
		}
	}
	out[i] = '\0';
	return i;
}

int main(int argc, char **argv) {
	char *input = "1113122113";
	if (argc > 1) input = argv[1];
	int cap = 2048;
	char *in = malloc(cap * 2);
	char *out = malloc(cap * 2);
	if (!in || !out) return 1;
	strcpy(in, input);
	int res = strlen(in);
	for (int i = 0; i < 50; i++) {
		if (i == 40) printf("Day 10, part 1: %d\n", res);
		if ((res = rle(in, out)) > cap) {
			cap *= 2;
			in = realloc(in, cap * 2);
			out = realloc(out, cap * 2);
			if (!in || !out) {
				perror("allocating");
				return 1;
			}
		}
		char *temp = in;
		in = out;
		out = temp;
	}
	printf("Day 10, part 2: %d\n", res);
	free(in);
	free(out);
}

