#include "automata.h"

#ifdef DEBUG
#include <iostream>

inline void print_cell(CELL v, bool highlighted) {
	if (v) {
		if (highlighted) {
			std::cout << "I";
		} else {
			std::cout << "1";
		}
	} else {
		if (highlighted) {
			std::cout << "0";
		} else {
			std::cout << ".";
		}
	}
}

void print_state(
	CELL row_buf[][2],
	CELL neigh_buf[][3],
	CELL curr_in,
	World *w_in,
	World *w_out,
	WLD_X_COORD x_in,
	WLD_Y_COORD y_in,
	WLD_X_COORD x_out,
	WLD_Y_COORD y_out,
	WLD_X_COORD x_row_buf
) {

	std::cout << "x_in: " << (unsigned long) x_in << ", ";
	std::cout << "y_in: " << (unsigned long) y_in << ", ";
	std::cout << "x_out: " << (unsigned long) x_out << ", ";
	std::cout << "y_out: " << (unsigned long) y_out << ", ";
	std::cout << "x_row_buf: " << (unsigned long) x_row_buf << std::endl;

	for (char i = 0; i < 3; i++) {
		print_cell(neigh_buf[i][0], false);
	}
	std::cout << " - ";
	for (WLD_BIG_COORD i = 0; i < WLD_W; i++) {
		print_cell(row_buf[i][0], i == x_row_buf);
	}
	std::cout << std::endl;

	for (char i = 0; i < 3; i++) {
		print_cell(neigh_buf[i][1], false);
	}
	std::cout << " - ";
	for (WLD_BIG_COORD i = 0; i < WLD_W; i++) {
		print_cell(row_buf[i][1], i == x_row_buf);
	}
	std::cout << std::endl;

	for (char i = 0; i < 3; i++) {
		print_cell(neigh_buf[i][2], false);
	}
	std::cout << " - ";
	print_cell(curr_in, false);
	std::cout << std::endl;

	std::cout << std::endl;

	for (WLD_BIG_COORD y = 0; y < WLD_H; y++) {
		for (WLD_BIG_COORD x = 0; x < WLD_W; x++) {
			print_cell(wld_get(w_out, x, y), (y == y_out) && (x == x_out));
		}
		std::cout << " - ";
		for (WLD_BIG_COORD x = 0; x < WLD_W; x++) {
			print_cell(wld_get(w_in, x, y), (y == y_in) && (x == x_in));
		}
		std::cout << std::endl;
	}

	std::cout << "~~~~~~~~~~~~" << std::endl;
}
#endif

// Solution: initial -------------------------------------------------

/*
int automata_hw(World *w_in, World *w_out) {

	// Buffer with the last two rows
	CELL row_buf[WLD_W][2];
	for (WLD_BIG_COORD i = 0; i < WLD_W; i++) {
		row_buf[i][0] = 0; // TODO: How to remove?
		row_buf[i][1] = 0;
	}
	// Buffer with the 3x3 neighborhood
	CELL neigh_buf[3][3]; // Not necessary to init

	// Currently read value and coordinates
	CELL curr_in = false;
	WLD_X_COORD x_in = 0;
	WLD_Y_COORD y_in = 0;

	// Value currently being saved
	WLD_X_COORD x_out = 0;
	WLD_Y_COORD y_out = 0;

	// Current position in row_buf
	WLD_X_COORD x_row_buf = 0;

	main_loop: for (unsigned int i = 0; i < WLD_W * (WLD_H + 1) + 2; i++) {
#pragma HLS PIPELINE
		curr_in = wld_get(w_in, x_in, y_in);

		// Operate in neighborhood

		// Or maybe think if I can do it with some combination of XOR?
		// TODO can be smaller type
		char neighs = neigh_buf[0][0] + neigh_buf[0][1] + neigh_buf[0][2]
					+ neigh_buf[1][0]                   + neigh_buf[1][2]
					+ neigh_buf[2][0] + neigh_buf[2][1] + neigh_buf[2][2];

		if (neigh_buf[1][1]) {
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

		if (x_in == WLD_W - 1) {
			x_in = 0;
			y_in++;
		} else {
			x_in++;
		}

		// Iterate row buffer coordinates

		x_row_buf = x_in;

		// Iterate output coordinates

		// TODO good idea?
		if (x_out == WLD_W - 1) {
			x_out = 0;
			y_out++;
		} else {
			x_out++;
		}
		if (i < WLD_W + 2) {
			x_out = 0;
		}
	}

	return 0;
}
*/

// Solution: axi_stream -------------------------------------------------

int automata_hw(hls::stream<CELL> &in_stream, hls::stream<CELL> &out_stream) {
#pragma HLS INTERFACE axis register both port=in_stream
#pragma HLS INTERFACE axis register both port=out_stream
#pragma HLS INTERFACE s_axilite port=return

	// Buffer with the last two rows
	static CELL row_buf[WLD_W][2]; // Static so it initializes to 0

	// Buffer with the 3x3 neighborhood
	CELL neigh_buf[3][3]; // Not necessary to init

	// Currently read value and coordinates
	CELL curr_in = false;

	// Current position in row_buf
	WLD_X_COORD x_row_buf = 0;

	// Current position in output image
	WLD_X_COORD x_out = 0;
	WLD_X_COORD y_out = 0;

	main_loop: for (unsigned int i = 0; i < WLD_W * (WLD_H + 1) + 2; i++) {
#pragma HLS PIPELINE
		curr_in = in_stream.read();

		// Operate in neighborhood

		ap_uint<4> neighs = neigh_buf[0][0] + neigh_buf[0][1] + neigh_buf[0][2]
					      + neigh_buf[1][0]                   + neigh_buf[1][2]
					      + neigh_buf[2][0] + neigh_buf[2][1] + neigh_buf[2][2];

		if (i > WLD_W + 1) {
			// If the input data reached the neigh_buf

			if (x_out != 0 && x_out != WLD_W - 1
					&& y_out != 0 && y_out != WLD_H - 1) {
				// If it is not in margin

				if (neigh_buf[1][1]) {
					// If the cell was alive

					out_stream.write(neighs == 2 || neighs == 3);
				} else {
					// If the cell was dead

					out_stream.write(neighs == 3);
				}
			} else {
				// If it is in margin

				out_stream.write(0); // Write margins as zero
			}
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

		// Iterate row buffer coordinates

		if (x_row_buf == WLD_W - 1) {
			x_row_buf = 0;
		} else {
			x_row_buf++;
		}

		// Iterate output image coordinates

		if (x_out == WLD_W - 1) {
			x_out = 0;
			y_out++;
		} else {
			if (i > WLD_W + 1) {
				x_out++;
			}
		}
	}

	return 0;
}

