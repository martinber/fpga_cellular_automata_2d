#include "automata.h"

#ifdef DEBUG
#include <iostream>
#endif

void print_state(
	bool row_buf[][2],
	bool neigh_buf[][3],
	bool curr_in,
	World *w_in,
	World *w_out,
	WLD_COORD x_in,
	WLD_COORD y_in,
	WLD_COORD x_out,
	WLD_COORD y_out,
	WLD_COORD x_row_buf
) {
#ifdef DEBUG
	std::cout << "x_in: " << (unsigned long) x_in << ", ";
	std::cout << "y_in: " << (unsigned long) y_in << ", ";
	std::cout << "x_out: " << (unsigned long) x_out << ", ";
	std::cout << "y_out: " << (unsigned long) y_out << ", ";
	std::cout << "x_row_buf: " << (unsigned long) x_row_buf << std::endl;
#endif
}

int automata_hw(World *w_in, World *w_out) {


	// Buffer with the last two rows
	bool row_buf[WLD_W][2];
	// Buffer with the 3x3 neighborhood
	bool neigh_buf[3][3] = {
		{false, false, false},
		{false, false, false},
		{false, false, false}
	};

	// Currently read value and coordinates
	bool curr_in;
	WLD_COORD x_in = 0;
	WLD_COORD y_in = 0;

	// Value currently being saved
	WLD_COORD x_out = 0; // TODO should be different
	WLD_COORD y_out = 0; // TODO should be different

	// Current position in row_buf
	WLD_COORD x_row_buf = 0; // TODO should be different

	bool finish = false;

	while (!finish) {

		print_state(
			row_buf, neigh_buf, curr_in, w_in, w_out,
			x_in, y_in, x_out, y_out, x_row_buf
		);

		curr_in = wld_get(w_in, x_in, x_out);

		// Operate in neighborhood

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

		neigh_buf[0][0] = neigh_buf[1][0];
		neigh_buf[0][1] = neigh_buf[1][1];
		neigh_buf[0][2] = neigh_buf[1][2];

		neigh_buf[1][0] = neigh_buf[2][0];
		neigh_buf[1][1] = neigh_buf[2][1];
		neigh_buf[1][2] = neigh_buf[2][2];

		neigh_buf[2][0] = row_buf[x_row_buf][0];
		neigh_buf[2][1] = row_buf[x_row_buf][1];
		neigh_buf[2][2] = curr_in;

		// Update row_buf

		row_buf[x_row_buf][0] = row_buf[x_row_buf][1];
		row_buf[x_row_buf][1] = curr_in;

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

		x_row_buf++;
		if (x_row_buf == WLD_W) {
			x_row_buf = 0;
		}

		// Iterate output coordinates

		// TODO should be different
		x_out++;
		if (x_out == WLD_W) {
			x_out = 0;
			y_out++;
		}
	}

	return 0;
}
