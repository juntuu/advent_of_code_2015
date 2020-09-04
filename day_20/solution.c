
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

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

int robins_inequality(int x) {
	double n = x;
	return exp(0.57721566490153286060651209008240243104215933593992) * n * log(log(n));
}

int find_starting_point(int target) {

	int lo = 5040;
	int hi = target;
	while (lo < hi) {
		int mid = (lo + hi) / 2;
		int val = robins_inequality(mid);
		if (val < target)
			lo = mid + 1;
		else if (val > target)
			hi = mid;
	}
	return lo;
}

int p2(int target, int start) {
	const int MULTIPLIER = 11;
	const int STEPS = 50;
	int N = target / MULTIPLIER;
	int *house = calloc(N-start, sizeof(*house));
	if (!house) {
		perror("allocating houses");
		exit(1);
	}

	int found = target;
	for (int i = start / STEPS; i <= N; i++) {
		int j = i;
		int steps = STEPS;
		if (j <= start) {
			int times = 1 + (start - j) / i;
			j += i * times;
			steps -= times;
		}
		for (; steps && j <= N; j += i, steps--) {
			if ((house[j-1-start] += i * MULTIPLIER) >= target && j < found) {
				N = j;
				found = j;
			}
		}
	}
	free(house);
	return found;
}

int main() {
	int input = 34000000;
	int got = 0;
	int house = find_starting_point(input / 10);
	do {
		house++;
		got = 10 * factor_sum(house);
	} while (got < input);

	printf("Day 20, part 1: %d\n", house);

	house = p2(input, house);
	printf("Day 20, part 2: %d\n", house);
}

