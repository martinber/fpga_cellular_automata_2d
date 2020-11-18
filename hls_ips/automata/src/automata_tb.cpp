#include <iostream>
#include <cstdlib>
#include <vector>
#include "automata.h"


int automata_sw(World *w_in, World *w_out) {
	for (WLD_BIG_COORD x = 0; x < WLD_W; x++) {
		for (WLD_BIG_COORD y = 0; y < WLD_H; y++) {

			char neighbors = 0;
			if (x > 0 && y > 0) {
				if (wld_get(w_in, x - 1, y - 1)) {
					neighbors++;
				}
			}
			if (x > 0) {
				if (wld_get(w_in, x - 1, y)) {
					neighbors++;
				}
			}
			if (x > 0 && y < WLD_H - 1) {
				if (wld_get(w_in, x - 1, y + 1)) {
					neighbors++;
				}
			}
			if (y > 0) {
				if (wld_get(w_in, x, y - 1)) {
					neighbors++;
				}
			}

			if (y < WLD_H - 1) {
				if (wld_get(w_in, x, y + 1)) {
					neighbors++;
				}
			}
			if (x < WLD_W - 1 && y > 0) {
				if (wld_get(w_in, x + 1, y - 1)) {
					neighbors++;
				}
			}
			if (x < WLD_W - 1) {
				if (wld_get(w_in, x + 1, y)) {
					neighbors++;
				}
			}
			if (x < WLD_W - 1 && y < WLD_H - 1) {
				if (wld_get(w_in, x + 1, y + 1)) {
					neighbors++;
				}
			}

			if (wld_get(w_in, x, y)) {
				wld_set(w_out, x, y, neighbors == 2 || neighbors == 3);
			} else {
				wld_set(w_out, x, y, neighbors == 3);
			}
		}
	}
	return 0;
}

void print_world(World *world) {
	for (WLD_BIG_COORD y = 0; y < WLD_H; y++) {
		for (WLD_BIG_COORD x = 0; x < WLD_W; x++) {
			if (wld_get(world, x, y)) {
				std::cout << "1";
			} else {
				std::cout << ".";
			}
		}
		std::cout << std::endl;
	}
}

void init_random_world(World *world) {
	for (WLD_BIG_COORD y = 0; y < WLD_H; y++) {
		for (WLD_BIG_COORD x = 0; x < WLD_W; x++) {
			wld_set(world, x, y, rand() % 2);
		}
	}
}

void init_empty_world(World *world) {
	for (WLD_BIG_COORD y = 0; y < WLD_H; y++) {
		for (WLD_BIG_COORD x = 0; x < WLD_W; x++) {
			wld_set(world, x, y, 0);
		}
	}
}

/**
 * World with some live cells.
 *
 * Needs at least some size.
 */
void init_test_world(World *world) {
	for (WLD_BIG_COORD y = 0; y < WLD_H; y++) {
		for (WLD_BIG_COORD x = 0; x < WLD_W; x++) {
			wld_set(world, x, y, 0);
		}
	}
	wld_set(world, 0, 0, 1);
	wld_set(world, 1, 0, 1);
	wld_set(world, 0, 1, 1);
	wld_set(world, 1, 1, 1);

	wld_set(world, 5, 0, 1);
	wld_set(world, 5, 1, 1);
	wld_set(world, 5, 2, 1);
}

int unit_test(World *w_in) {
	World w_out_sw;
	World w_out_hw;
	init_empty_world(&w_out_sw);
	init_empty_world(&w_out_hw);

	std::cout << "Input world:" << std::endl;
	print_world(w_in);

	automata_sw(w_in, &w_out_sw);
	automata_hw(w_in, &w_out_hw);

	std::cout << "automata_sw output world:" << std::endl;
	print_world(&w_out_sw);
	std::cout << "automata_hw output world:" << std::endl;
	print_world(&w_out_hw);

	for (WLD_BIG_COORD y = 0; y < WLD_H; y++) {
		for (WLD_BIG_COORD x = 0; x < WLD_W; x++) {
			if (wld_get(&w_out_sw, x, y) != wld_get(&w_out_hw, x, y)) {
				return FAIL;
			}
		}
	}
	return OK;
}


int main() {
	srand(1234);

	World w_in;

	init_test_world(&w_in);

	int test_res = unit_test(&w_in);

	if (test_res == FAIL) {
		std::cout << "Test run failed!" << std::endl;
		return FAIL;
	} else {
		std::cout << "Test run OK!" << std::endl;
	}

	return OK;
}

