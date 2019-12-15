
#include <stdio.h>

int main() {
	int total = 0;
	int content = 0;
	int encoded = 0;
	int c;
	while ((c = getchar()) != EOF) {
		if (c == '\n') continue;
		total++;
		if (c == '"') {
			encoded += 2;
			continue;
		}
		if (c == '\\') {
			encoded++;
			total++;
			switch (getchar()) {
				case '"':
				case '\\':
					encoded++;
					break;
				case 'x':
					getchar();
					getchar();
					total += 2;
					break;
			}
		}
		content++;
	}
	printf("Day 8, part 1: %d\n", total - content);
	printf("Day 8, part 2: %d\n", encoded);
}
