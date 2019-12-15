
#include <stdio.h>

int main() {
	int total = 0;
	int content = 0;
	int c;
	while ((c = getchar()) != EOF) {
		if (c == '\n') continue;
		total++;
		if (c == '"') continue;
		if (c == '\\') {
			total++;
			if (getchar() == 'x') {
				getchar();
				getchar();
				total += 2;
			}
		}
		content++;
	}
	printf("Day 8, part 1: %d\n", total - content);
}
