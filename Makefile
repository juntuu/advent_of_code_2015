days := $(shell seq -w 1 25)

CC := clang
C_FLAGS := -Wall -Wextra -pedantic --std=c17 -O3

.PHONY: help clean $(days)

help:
	@echo "Targets:"
	@echo "       make DD <input  # build and run selected day with input from stdin"
	@echo "       make clean      # remove built executables"

$(days): %: day_%/a.out
	./day_$@/a.out

day_%/a.out: day_%/solution.c
	$(CC) $(C_FLAGS) -o $@ $<

clean:
	rm -f day_*/a.out
