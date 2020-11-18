#ifndef AUTOMATA_H
#define AUTOMATA_H

#include "ap_int.h"

// This flag is only useful in simulation or cosimulation
#define DEBUG

#ifdef DEBUG
#include <assert.h>
#endif

// Because I always forget which one
#define OK 0
#define FAIL 1

// World size has to be power of two because of optimizations
#define WLD_W_DEPTH 5
#define WLD_H_DEPTH 3
#define WLD_W 32 // Has to be 2^WLD_W_DEPTH
#define WLD_H 8 // Has to be 2^WLD_H_DEPTH

// Type used for world coordinates
typedef unsigned char WLD_COORD;
typedef ap_uint<WLD_W_DEPTH> WLD_X_COORD;
typedef ap_uint<WLD_H_DEPTH> WLD_Y_COORD;

// Type used for boolean values of each cell
typedef ap_uint<1> CELL;

/*
 * It represents the world of the game.
 *
 * There are getter and setter functions in case I change the representation
 * later
 */
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

// The main function
int automata_hw(World *world_in, World *world_out);

#endif
