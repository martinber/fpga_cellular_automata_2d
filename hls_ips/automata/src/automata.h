#ifndef AUTOMATA_H
#define AUTOMATA_H

#define OK 0
#define FAIL 1

#define WORLD_W 32
#define WORLD_H 8

// Type used for world coordinates
typedef unsigned char WORLD_COORD;

struct World {
	bool bit_array[WORLD_W][WORLD_H];
};

int automata_hw(World *world_in, World *world_out);

#endif
