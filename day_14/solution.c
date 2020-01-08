
#include <stdio.h>
#include <stdlib.h>

typedef struct {
	int speed;
	int fly;
	int rest;
} Reindeer;

int race(Reindeer r, int time) {
	int period = r.fly + r.rest;
	int distance = r.speed * r.fly * (time / period);
	int more = time % period;
	more = more > r.fly ? r.fly : more;
	return distance + more * r.speed;
}

int race2(Reindeer *rs, int n, int time) {
	int best = 0;
	int *points = calloc(n, sizeof(int));
	int *distances = calloc(n, sizeof(int));
	if (!points || !distances) {
		if (points) free(points);
		if (distances) free(distances);
		perror("allocating");
		return -1;
	}

	for (int t = 0; t < time; t++) {
		best = 0;
		for (int i = 0; i < n; i++) {
			int flying = t % (rs[i].fly + rs[i].rest) < rs[i].fly;
			distances[i] += rs[i].speed * flying;
			if (distances[i] > best) best = distances[i];
		}
		for (int i = 0; i < n; i++)
			points[i] += distances[i] == best;
	}
	best = 0;
	for (int i = 0; i < n; i++)
		best = points[i] > best ? points[i] : best;

	free(points);
	free(distances);
	return best;
}

int main(int argc, char **argv) {
	int cap = 9;
	Reindeer *reindeers = malloc(cap * sizeof(Reindeer));
	if (!reindeers) {
		perror("allocating");
		return 1;
	}
	int n = 0;
	int s, f, r;
	while (scanf("%*s can fly %d km/s for %d seconds, but then must rest for %d seconds.", &s, &f, &r) == 3) {
		if (n >= cap) {
			cap *= 2;
			reindeers = realloc(reindeers, cap * sizeof(Reindeer));
			if (!reindeers) {
				perror("allocating");
				return 1;
			}
		}
		reindeers[n++] = (Reindeer){s, f, r};
	}
	int seconds = 2503;
	if (argc > 1 && sscanf(argv[1], "%d", &s) == 1) {
		seconds = s;
	}
	int best = 0;
	for (int i = 0; i < n; i++) {
		int distance = race(reindeers[i], seconds);
		if (distance > best) best = distance;
	}
	printf("Day 14, part 1: %d\n", best);

	best = race2(reindeers, n, seconds);
	printf("Day 14, part 2: %d\n", best);

	free(reindeers);
}

