#include <iostream>
#include <cstdlib>
#include <vector>
#include "automata.h"

int automata_sw(CELL w_in[][WLD_H], CELL w_out[][WLD_H]) {
	for (WLD_BIG_COORD x = 0; x < WLD_W; x++) {
		for (WLD_BIG_COORD y = 0; y < WLD_H; y++) {

			char neighbors = 0;
			if (x > 0 && y > 0) {
				if (w_in[x - 1][y - 1]) {
					neighbors++;
				}
			}
			if (x > 0) {
				if (w_in[x - 1][y]) {
					neighbors++;
				}
			}
			if (x > 0 && y < WLD_H - 1) {
				if (w_in[x - 1][y + 1]) {
					neighbors++;
				}
			}
			if (y > 0) {
				if (w_in[x][y - 1]) {
					neighbors++;
				}
			}

			if (y < WLD_H - 1) {
				if (w_in[x][y + 1]) {
					neighbors++;
				}
			}
			if (x < WLD_W - 1 && y > 0) {
				if (w_in[x + 1][y - 1]) {
					neighbors++;
				}
			}
			if (x < WLD_W - 1) {
				if (w_in[x + 1][y]) {
					neighbors++;
				}
			}
			if (x < WLD_W - 1 && y < WLD_H - 1) {
				if (w_in[x + 1][y + 1]) {
					neighbors++;
				}
			}

			if (y > 0 && y < WLD_H - 1 && x > 0 && x < WLD_W - 1) {
				if (w_in[x][y]) {
					w_out[x][y] = neighbors == 2 || neighbors == 3;
				} else {
					w_out[x][y] = neighbors == 3;
				}
			} else {
				w_out[x][y] = 0; // Write 0 in margins
			}
		}
	}
	return 0;
}

void print_world(CELL world[][WLD_H]) {
	for (WLD_BIG_COORD y = 0; y < WLD_H; y++) {
		for (WLD_BIG_COORD x = 0; x < WLD_W; x++) {
			if (world[x][y]) {
				std::cout << "1";
			} else {
				std::cout << ".";
			}
		}
		std::cout << std::endl;
	}
}

void init_random_world(CELL world[][WLD_H]) {
	for (WLD_BIG_COORD y = 0; y < WLD_H; y++) {
		for (WLD_BIG_COORD x = 0; x < WLD_W; x++) {
			if (y > 0 && y < WLD_H - 1 && x > 0 && x < WLD_W - 1) {
				world[x][y] = rand() % 2;
			} else {
				world[x][y] = 0; // Write 0 in margins
			}
		}
	}
}

void init_empty_world(CELL world[][WLD_H]) {
	for (WLD_BIG_COORD y = 0; y < WLD_H; y++) {
		for (WLD_BIG_COORD x = 0; x < WLD_W; x++) {
			world[x][y] = 0;
		}
	}
}

int unit_test(CELL w_in[][WLD_H]) {
	CELL w_out_sw[WLD_W][WLD_H];
	init_empty_world(w_out_sw);

	std::cout << "Input world:" << std::endl;
	print_world(w_in);

	automata_sw(w_in, w_out_sw);

	World world_in;
	World world_out;
	for (WLD_BIG_COORD y = 0; y < WLD_H; y++) {
		for (WLD_BIG_COORD x = 0; x < WLD_W; x++) {
			world_in.array[x][y] = w_in[x][y];
		}
	}
	main_hw((int *) (world_in.array), (int *) (world_out.array));

	std::cout << "automata_sw output world:" << std::endl;
	print_world(w_out_sw);
	std::cout << "automata_hw output world:" << std::endl;
	print_world(world_out.array);

	for (WLD_BIG_COORD y = 0; y < WLD_H; y++) {
		for (WLD_BIG_COORD x = 0; x < WLD_W; x++) {
			if (w_out_sw[x][y] != world_out.array[x][y]) {
				return FAIL;
			}
		}
	}
	return OK;
}


int main() {

	CELL w_in[WLD_W][WLD_H];

	init_random_world(w_in);

	int test_res = unit_test(w_in);

	if (test_res == FAIL) {
		std::cout << "Test run failed!" << std::endl;
		return FAIL;
	} else {
		std::cout << "Test run OK!" << std::endl;
	}

	return OK;
}

