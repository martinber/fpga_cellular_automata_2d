#include "automata.h"

int automata_hw(World *world_in, World *world_out) {
	for (WORLD_COORD x = 0; x < WORLD_W; x++) {
		for (WORLD_COORD y = 0; y < WORLD_H; y++) {
			world_out->bit_array[x][y] = world_in->bit_array[x][y];
		}
	}
	return 0;
}
