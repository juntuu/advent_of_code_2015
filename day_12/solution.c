
#include <stdio.h>

int is_red(FILE *f) {
	char ref[] = "asd";
	int c;
	int i = 0;
	while ((c = getc(f)) != '"') {
		if (i < 3) ref[i] = c;
		i++;
	}
	int res = i == 3 && ref[0] == 'r' && ref[1] == 'e' && ref[2] == 'd';
	return res;
}

typedef struct {
	int sum;
	int red;
} Result;

Result json_sum(FILE *f) {
	Result res;
	int sum = 0;
	int red = 0;
	int num = 0;
	int sign = 1;
	int c;
	int last = EOF;
	while ((c = getc(f)) != EOF) {
		if ('0' <= c && c <= '9') {
			num *= 10;
			num += c - '0';
			last = c;
			continue;
		}
		if (num) {
			sum += num * sign;
			num = 0;
			sign = 1;
		}
		switch (c) {
			case '{':
			case '[':
				res = json_sum(f);
				sum += res.sum;
				red += res.red;
				break;
			case '"':
				if (is_red(f) && last == ':') {
					res = json_sum(f);
					res.red += res.sum + sum + red;
					res.sum = 0;
					return res;
				}
				break;
			case ']':
			case '}': return (Result) {sum, red};
			case '-': sign = -1; break;
		}
		last = c;
	}
	return (Result) {sum + num * sign, red};
}

int main() {
	Result res = json_sum(stdin);
	printf("Day 12, part 1: %d\n", res.sum + res.red);
	printf("Day 12, part 2: %d\n", res.sum);
}

