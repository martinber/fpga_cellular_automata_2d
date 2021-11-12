#ifndef AUTOMATA_H
#define AUTOMATA_H

#include "ap_int.h"
#include "hls_stream.h"

// Because I always forget which one
#define OK 0
#define FAIL 1

#define WLD_W_DEPTH 8
#define WLD_H_DEPTH 8
#define WLD_W 250 // Has to be max 2^WLD_W_DEPTH
#define WLD_H 250 // Has to be max 2^WLD_H_DEPTH

// Type used for world coordinates
typedef ap_uint<WLD_W_DEPTH+1> WLD_BIG_COORD; // Has extra space, because when writing loops you need for the condition
typedef ap_uint<WLD_W_DEPTH> WLD_X_COORD; // Coordinates barely fits
typedef ap_uint<WLD_H_DEPTH> WLD_Y_COORD; // Coordinates barely fits

// Type used for boolean values of each cell
typedef int CELL;


// Data structure to pass to HW function
struct World {
	CELL array[WLD_W][WLD_H];
};

// The main function
void main_hw(CELL *w_in, CELL *w_out);
//int main_hw(World *w_in, World *w_out);



#endif
