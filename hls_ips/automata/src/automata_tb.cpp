#include <iostream>
#include <cstdlib>
#include <vector>
#include "automata.h"


int automata_sw(World *world_in, World *world_out) {
	for (WORLD_COORD x = 0; x < WORLD_W; x++) {
		for (WORLD_COORD y = 0; y < WORLD_H; y++) {
			world_out->bit_array[x][y] = world_in->bit_array[x][y];
		}
	}
	return 0;
}

void print_world(World *world) {
	for (WORLD_COORD y = 0; y < WORLD_H; y++) {
		for (WORLD_COORD x = 0; x < WORLD_W; x++) {
			if (world->bit_array[x][y]) {
				std::cout << "1";
			} else {
				std::cout << ".";
			}
		}
		std::cout << std::endl;
	}
}

void init_world(World *world) {
	for (WORLD_COORD y = 0; y < WORLD_H; y++) {
		for (WORLD_COORD x = 0; x < WORLD_W; x++) {
			world->bit_array[x][y] = rand() % 2;
		}
	}
}

int unit_test(World *world_in) {
	World world_out_sw;
	World world_out_hw;

	std::cout << "Input world:" << std::endl;
	print_world(world_in);

	automata_sw(world_in, &world_out_sw);
	automata_hw(world_in, &world_out_hw);

	std::cout << "automata_sw output world:" << std::endl;
	print_world(&world_out_sw);
	std::cout << "automata_hw output world:" << std::endl;
	print_world(&world_out_hw);

	for (WORLD_COORD y = 0; y < WORLD_H; y++) {
		for (WORLD_COORD x = 0; x < WORLD_W; x++) {
			if (world_out_sw.bit_array[x][y] != world_out_hw.bit_array[x][y]) {
				return FAIL;
			}
		}
	}
	return OK;
}


int main() {
	srand(1234);

	World world_in;

	init_world(&world_in);

	int test_res = unit_test(&world_in);

	if (test_res == FAIL) {
		std::cout << "Test run failed!" << std::endl;
		return FAIL;
	} else {
		std::cout << "Test run OK!" << std::endl;
	}

	return OK;
}

