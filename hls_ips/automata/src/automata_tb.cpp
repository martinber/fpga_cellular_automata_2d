#include <iostream>
#include <cstdlib>
#include <vector>
#include "automata.h"

/*
 * It represents the world of the game.
 *
 * There are getter and setter functions in case I change the representation
 * later
 */
struct World {
	CELL _bit_array[WLD_W][WLD_H];
};

inline bool wld_get(World *world, WLD_X_COORD x, WLD_Y_COORD y) {
#ifdef DEBUG
	assert(x < WLD_W && y < WLD_H);
#endif

	return world->_bit_array[x][y];
}

inline void wld_set(World *world, WLD_X_COORD x, WLD_Y_COORD y, bool value) {
#ifdef DEBUG
	assert(x < WLD_W && y < WLD_H);
#endif

	world->_bit_array[x][y] = value;
}


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

			if (y > 0 && y < WLD_H - 1 && x > 0 && x < WLD_W - 1) {
				if (wld_get(w_in, x, y)) {
					wld_set(w_out, x, y, neighbors == 2 || neighbors == 3);
				} else {
					wld_set(w_out, x, y, neighbors == 3);
				}
			} else {
				wld_set(w_out, x, y, 0); // Write 0 in margins
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
			if (y > 0 && y < WLD_H - 1 && x > 0 && x < WLD_W - 1) {
				wld_set(world, x, y, rand() % 2);
			} else {
				wld_set(world, x, y, 0); // Write 0 in margins
			}
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
	wld_set(world, 1, 1, 1);
	wld_set(world, 2, 1, 1);
	wld_set(world, 1, 2, 1);
	wld_set(world, 2, 2, 1);

	wld_set(world, 5, 1, 1);
	wld_set(world, 5, 2, 1);
	wld_set(world, 5, 3, 1);

	wld_set(world, WLD_W - 3, WLD_H - 3, 1);
	wld_set(world, WLD_W - 2, WLD_H - 3, 1);
	wld_set(world, WLD_W - 3, WLD_H - 2, 1);
	wld_set(world, WLD_W - 2, WLD_H - 2, 1);
}

void run_automata_hw(World *w_in, World *w_out) {
	hls::stream<CELL> in_stream;
	hls::stream<CELL> out_stream;

	for (WLD_BIG_COORD y = 0; y < WLD_H; y++) {
		for (WLD_BIG_COORD x = 0; x < WLD_W; x++) {
			in_stream.write(wld_get(w_in, x, y));
		}
	}

	automata_hw(in_stream, out_stream);
	//automata_hw(w_in, w_out);

	for (WLD_BIG_COORD y = 0; y < WLD_H; y++) {
		for (WLD_BIG_COORD x = 0; x < WLD_W; x++) {
			wld_set(w_out, x, y, out_stream.read());
		}
	}
}

int unit_test(World *w_in) {
	World w_out_sw;
	World w_out_hw;
	init_empty_world(&w_out_sw);
	init_empty_world(&w_out_hw);

	std::cout << "Input world:" << std::endl;
	print_world(w_in);

	automata_sw(w_in, &w_out_sw);
	run_automata_hw(w_in, &w_out_hw);

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

	init_random_world(&w_in);

	int test_res = unit_test(&w_in);

	if (test_res == FAIL) {
		std::cout << "Test run failed!" << std::endl;
		return FAIL;
	} else {
		std::cout << "Test run OK!" << std::endl;
	}

	return OK;
}

