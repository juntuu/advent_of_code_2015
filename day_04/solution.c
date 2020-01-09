
#include <stdio.h>
#include <string.h>
#include <pthread.h>

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

typedef struct {
	int *done;
	int *winner;
	char str[32];
	int start;
	int from;
	int step;
	unsigned char *mask;
	int len;
} X;

void *brute_worker(void *p) {
	X *x = p;
	int n = sprintf(x->str+x->start, "%d", x->from);
	int res = x->from;
	unsigned char digest[16];
	int winner = 0;
	while (!winner) {
		CC_MD5(x->str, x->start + n, digest);
		winner = res;
		for (int i = 0; i < x->len; i++) {
			if (digest[i] & x->mask[i]) {
				winner = 0;
				break;
			}
		}
		for (int i = 0; i < x->step; i++)
			n = step(x->str+x->start, n);
		res += x->step;
		if (*x->done && *x->done < res)
			break;
	}
	*x->winner = winner;
	if (!*x->done) *x->done = winner;
	return NULL;
}

#ifndef NUM_THREADS
#define NUM_THREADS 4
#endif

int brute_force_par(char *str, int start, int from, unsigned char *mask, int len) {
	X xs[NUM_THREADS] = {};
	int winners[NUM_THREADS] = {};
	int done = 0;
	pthread_t ts[NUM_THREADS-1];
	for (int i = 0; i < NUM_THREADS; i++) {
		xs[i].done = &done;
		xs[i].from = from + i;
		xs[i].len = len;
		xs[i].mask = mask;
		xs[i].start = start;
		xs[i].step = NUM_THREADS;
		strncpy(xs[i].str, str, 32);
		xs[i].winner = &winners[i];
		if (i) pthread_create(&ts[i-1], NULL, brute_worker, (void*)&xs[i]);
	}
	brute_worker((void*)&xs[0]);
	int winner = 0;
	for (int i = 0; i < NUM_THREADS; i++) {
		if (i) pthread_join(ts[i-1], NULL);
		int x = winners[i];
		if (winner == 0) winner = x;
		else if (x && x < winner) winner = x;
	}
	return winner;
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
	int one = brute_force_par(str, n, 0, mask, 3);
	printf("Day 4, part 1: %d\n", one);
	mask[2] = 0xff;
	int two = brute_force_par(str, n, one, mask, 3);
	printf("Day 4, part 2: %d\n", two);
}

