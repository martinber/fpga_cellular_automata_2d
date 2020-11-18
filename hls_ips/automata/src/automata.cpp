#include "automata.h"

#ifdef DEBUG
#include <iostream>

inline void print_cell(CELL v) {
	if (v) {
		std::cout << "1";
	} else {
		std::cout << ".";
	}
}

void print_state(
	CELL row_buf[][2],
	CELL neigh_buf[][3],
	CELL curr_in,
	World *w_in,
	World *w_out,
	WLD_COORD x_in,
	WLD_COORD y_in,
	WLD_COORD x_out,
	WLD_COORD y_out,
	WLD_COORD x_row_buf
) {

	std::cout << "x_in: " << (unsigned long) x_in << ", ";
	std::cout << "y_in: " << (unsigned long) y_in << ", ";
	std::cout << "x_out: " << (unsigned long) x_out << ", ";
	std::cout << "y_out: " << (unsigned long) y_out << ", ";
	std::cout << "x_row_buf: " << (unsigned long) x_row_buf << std::endl;

	for (char i = 0; i < 3; i++) {
		print_cell(neigh_buf[i][0]);
	}
	std::cout << " - ";
	for (WLD_COORD i = 0; i < WLD_W; i++) {
		print_cell(row_buf[i][0]);
	}
	std::cout << std::endl;

	for (char i = 0; i < 3; i++) {
		print_cell(neigh_buf[i][1]);
	}
	std::cout << " - ";
	for (WLD_COORD i = 0; i < WLD_W; i++) {
		print_cell(row_buf[i][1]);
	}
	std::cout << std::endl;

	for (char i = 0; i < 3; i++) {
		print_cell(neigh_buf[i][2]);
	}
	std::cout << " - ";
	print_cell(curr_in);
	std::cout << std::endl;

	std::cout << std::endl;

	for (WLD_COORD y = 0; y < WLD_H; y++) {
		for (WLD_COORD x = 0; x < WLD_W; x++) {
			print_cell(wld_get(w_out, x, y));
		}
		std::cout << " - ";
		for (WLD_COORD x = 0; x < WLD_W; x++) {
			print_cell(wld_get(w_in, x, y));
		}
		std::cout << std::endl;
	}

	std::cout << "~~~~~~~~~~~~" << std::endl;
}
#endif

int automata_hw(World *w_in, World *w_out) {

	// Buffer with the last two rows
	CELL row_buf[WLD_W][2];
	for (WLD_COORD i = 0; i < WLD_W; i++) {
		row_buf[i][0] = 0; // TODO: Unnecessary?
		row_buf[i][1] = 0;
	}
	// Buffer with the 3x3 neighborhood
	CELL neigh_buf[3][3]; // Not necessary to init

	// Currently read value and coordinates
	CELL curr_in = false;
	WLD_COORD x_in = 0;
	WLD_COORD y_in = 0;

	// Value currently being saved
	WLD_COORD x_out = 0; // TODO should be different
	WLD_COORD y_out = 0; // TODO should be different

	// Current position in row_buf
	WLD_COORD x_row_buf = 0; // TODO should be different

	bool finish = false;

	/*
	// Goes an extra few loops to finish processing
	y_loop: for (WLD_COORD y_in; y_in < WLD_H + 1; y_in++) {
		x_loop: for (WLD_COORD x_in; x_in < WLD_W + 1; x_in++) {


		}
	}*/

	main_loop: while (!finish) {
		curr_in = wld_get(w_in, x_in, y_in);

		// Operate in neighborhood

		// TODO: Can do neighs += neigh_buf[0][0] + neigh_buf[0][1]...
		// Or maybe think if I can do it with some combination of XOR?
		char neighs = 0; // TODO can be smaller type
		if (neigh_buf[0][0]) neighs++;
		if (neigh_buf[0][1]) neighs++;
		if (neigh_buf[0][2]) neighs++;
		if (neigh_buf[1][0]) neighs++;
		// Central
		if (neigh_buf[1][2]) neighs++;
		if (neigh_buf[2][0]) neighs++;
		if (neigh_buf[2][1]) neighs++;
		if (neigh_buf[2][2]) neighs++;

		if (neigh_buf[1][1]) { // Central
			wld_set(w_out, x_out, y_out, neighs == 2 || neighs == 3);
		} else {
			wld_set(w_out, x_out, y_out, neighs == 3);
		}

		// Update neigh_buf

		// The compiler does not use a shift register here

		shift_neigh: for (int i = 0; i < 2; i++) {
#pragma HLS unroll
			neigh_buf[i][0] = neigh_buf[i+1][0];
			neigh_buf[i][1] = neigh_buf[i+1][1];
			neigh_buf[i][2] = neigh_buf[i+1][2];
		}

		neigh_buf[2][0] = row_buf[x_row_buf][0];
		neigh_buf[2][1] = row_buf[x_row_buf][1];
		neigh_buf[2][2] = curr_in;

		// Update row_buf

		row_buf[x_row_buf][0] = row_buf[x_row_buf][1];
		row_buf[x_row_buf][1] = curr_in;

		// Print the state for debugging

#ifdef DEBUG
		print_state(
			row_buf, neigh_buf, curr_in, w_in, w_out,
			x_in, y_in, x_out, y_out, x_row_buf
		);
#endif

		// Iterate input coordinates

		x_in++;
		if (x_in == WLD_W) {
			x_in = 0;
			y_in++;
		}
		if (y_in == WLD_H) {
			finish = true; // TODO: iterate two cycles more
		}

		// Iterate row buffer coordinates

		x_row_buf = x_in; // TODO: good idea?

		// Iterate output coordinates

		// TODO good idea?
		x_out++;
		if (y_in == 0 || (y_in == 1 && x_in < 3)) {
			x_out = 0;
		}
		if (x_out == WLD_W) {
			x_out = 0;
			y_out++;
		}
	}

	return 0;
}
