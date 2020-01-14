
#include <stdio.h>
#include <stdlib.h>

int find(int x, int *xs, int n) {
	if (n < 1) return -1;

	int mid = n/2;
	if (xs[mid] == x) return xs[mid+1];
	if (xs[mid] < x)
		return find(x, xs+mid+1, n-mid-1);
	return find(x, xs, mid);
}

int next_prime(int n) {
	static int cap = 0;
	static int m = 0;
	static int *primes = NULL;
	if (!primes) {
		cap = 32;
		primes = malloc(cap * sizeof(int));
		if (!primes) {
			perror("allocating primes");
			exit(1);
		}
		primes[m++] = 2;
		primes[m++] = 3;
	}
	if (n < primes[m-1]) return find(n, primes, m);
	if (cap <= m) {
		cap *= 2;
		primes = realloc(primes, cap * sizeof(int));
		if (!primes) {
			perror("allocating primes");
			exit(1);
		}
	}
	for (int i = primes[m-1]+2;; i+=2) {
		int p = 1;
		for (int j = 0; p && j < m; j++) p *= i % primes[j];
		if (p) {
			primes[m++] = i;
			return i;
		}
	}
}

int sum_pow(int x, int y) {
	int p = 1;
	int res = 1;
	while (y--) res += p *= x;
	return res;
}

int factor_sum(int n) {
	int sum = 1;
	int p = 2;
	while (n >= p) {
		int i = 0;
		while (n % p == 0) {
			i++;
			n /= p;
		}
		if (i > 0) sum *= sum_pow(p, i);
		p = next_prime(p);
	}
	return sum;
}

int factor_sum_up_to(int n, int max) {
	int sum = 0;
	for (int i = 1; i < max; i++)
		if (n % i == 0) sum += i;
	return sum;
}

int main() {
	int input = 34000000;
	int got = 0;
	int house = 0;
	do {
		house++;
		got = 10 * factor_sum(house);
	} while (got < input);
	printf("Day 20, part 1: %d\n", house);

	do {
		house++;
		got = factor_sum(house);
		got -= factor_sum_up_to(house, house/50);
		got *= 11;
	} while (got < input);
	printf("Day 20, part 2: %d\n", house);
}

