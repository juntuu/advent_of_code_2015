
#include <stdio.h>

int json_sum(FILE *f) {
	int sum = 0;
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
			case '{': sum += json_sum(f); break;
			case '[': sum += json_sum(f); break;
			case '"': while (getc(f) != '"') ; break;
			case ']': return sum;
			case '}': return sum;
			case '-': sign = -1; break;
		}
		last = c;
	}
	return sum + num * sign;
}

int main() {
	int sum = json_sum(stdin);
	printf("Day 12, part 1: %d\n", sum);
}

