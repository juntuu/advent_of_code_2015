#include <stdio.h>

char *step(char *s) {
	for (int i = 7; i >= 0; i--) {
		if (++s[i] > 'z') {
			s[i] = 'a';
			if (i == 0) return NULL;
			continue;
		}
		switch (s[i]) {
			case 'i':
			case 'o':
			case 'l':
				++s[i];
				while (++i < 8) s[i] = 'a';
		}
		break;
	}
	return s;
}

int valid(char *pw) {
	char s[3] = {*pw, '\0', '\0'};
	int si = 0;
	char p[2] = {*pw, '\0'};
	int pi = 0;
	for (int i = 1; i < 8 && si < 3 && pi < 2; i++) {
		if (si < 2) {
			if (s[si] == pw[i]-1) si++;
			else si = 0;
			s[si] = pw[i];
		}
		if (pi < 2) {
			if (p[pi] == pw[i] && (pi == 0 || p[0] != pw[i])) pi++;
			else p[pi] = pw[i];
		}
	}
	return si == 2 && pi == 2;
}

char *next_password(char *pw) {
	if (!pw) return NULL;
	int a = 0;
	for (char *c = pw; *c; c++) {
		if (a) *c = 'a';
		else switch (*c) {
			case 'i':
			case 'o':
			case 'l':
				(*c)++;
				a = 1;
		}
	}
	do pw = step(pw);
	while(pw && !valid(pw));
	return pw;
}

int main(int argc, char **argv) {
	char input[] = "cqjxjnds";
	char *password = input;
	if (argc > 1) password = argv[1];
	int n = 0;
	for (;; n++) {
		int c = password[n];
		if (c == '\0' || n > 8) break;
		if (c < 'a' || 'z' < c) {
			fprintf(stderr, "password must only contain lowercase characters\n");
			return 1;
		}
	}
	if (n != 8) {
		fprintf(stderr, "password must be exactly 8 characters, given password is too %s\n", n > 8 ? "long" : "short");
		return 1;
	}

	password = next_password(password);
	printf("Day 11, part 1: %s\n", password);

	password = next_password(password);
	printf("Day 11, part 2: %s\n", password);
}

