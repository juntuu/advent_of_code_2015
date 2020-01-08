
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

	free(reindeers);
}

