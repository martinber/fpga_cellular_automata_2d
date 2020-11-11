#ifndef AUTOMATA_H
#define AUTOMATA_H

#define DEBUG

#ifdef DEBUG
#include <assert.h>
#endif

#define OK 0
#define FAIL 1

#define WLD_W 32
#define WLD_H 8

// Type used for world coordinates
typedef unsigned char WLD_COORD;

struct World {
	bool _bit_array[WLD_W][WLD_H];
};

inline bool wld_get(World *world, WLD_COORD x, WLD_COORD y) {
#ifdef DEBUG
	assert(x < WLD_W && y < WLD_H);
#endif

	return world->_bit_array[x][y];
}

inline void wld_set(World *world, WLD_COORD x, WLD_COORD y, bool value) {
#ifdef DEBUG
	assert(x < WLD_W && y < WLD_H);
#endif

	world->_bit_array[x][y] = value;
}

int automata_hw(World *world_in, World *world_out);

#endif
