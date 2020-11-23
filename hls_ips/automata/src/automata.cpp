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

/* Solution: initial -------------------------------------------------

int automata_hw(World *w_in, World *w_out) {

	// Buffer with the last two rows
	CELL row_buf[WLD_W][2];
	for (WLD_BIG_COORD i = 0; i < WLD_W; i++) {
		row_buf[i][0] = 0; // TODO: Unnecessary?
		row_buf[i][1] = 0;
	}
	// Buffer with the 3x3 neighborhood
	CELL neigh_buf[3][3]; // Not necessary to init

	// Currently read value and coordinates
	CELL curr_in = false;

	bool finish = false;

	// Goes an extra few loops to finish processing
	y_loop: for (WLD_BIG_COORD y_in = 0; y_in < WLD_H + 1; y_in++) {
		x_loop: for (WLD_BIG_COORD x_in = 0; x_in < WLD_W; x_in++) {

			WLD_X_COORD x_row_buf = x_in;
			WLD_X_COORD x_out = x_in - 2;
			WLD_Y_COORD y_out = y_in - 1;

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
		}
	}

	return 0;
}
*/

/* Solution: one_loop -------------------------------------------------

int automata_hw(World *w_in, World *w_out) {

	// Buffer with the last two rows
	CELL row_buf[WLD_W][2];
	for (WLD_BIG_COORD i = 0; i < WLD_W; i++) {
		row_buf[i][0] = 0; // TODO: Unnecessary?
		row_buf[i][1] = 0;
	}
	// Buffer with the 3x3 neighborhood
	CELL neigh_buf[3][3];
    // Not necessary to init

	// Currently read value and coordinates
	CELL curr_in = false;

	bool finish = false;

	// Goes an extra few loops to finish processing
	main_loop: for (unsigned int i = 0; i < WLD_W * (WLD_H + 1); i++) {
#pragma HLS PIPELINE
		WLD_X_COORD x_in = i; // There is truncation but its ok
		WLD_X_COORD y_in = i / WLD_W; // Divide and round down

		WLD_X_COORD x_row_buf = x_in;
		WLD_X_COORD x_out = x_in - 2;
		WLD_Y_COORD y_out = y_in - 1;

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
	}

	return 0;
}

*/

/* Solution: axi_stream ------------------------------------------------- */

int automata_hw(World *w_in, World *w_out) {

	// Buffer with the last two rows
	CELL row_buf[WLD_W][2];
	for (WLD_BIG_COORD i = 0; i < WLD_W; i++) {
		row_buf[i][0] = 0; // TODO: Unnecessary?
		row_buf[i][1] = 0;
	}
	// Buffer with the 3x3 neighborhood
	CELL neigh_buf[3][3];
    // Not necessary to init

	// Currently read value and coordinates
	CELL curr_in = false;

	bool finish = false;

	// Goes an extra few loops to finish processing
	main_loop: for (unsigned int i = 0; i < WLD_W * (WLD_H + 1) + 2; i++) {
#pragma HLS PIPELINE
		WLD_X_COORD x_in = i; // There is truncation but its ok
		WLD_X_COORD y_in = i / WLD_W; // Divide and round down

		WLD_X_COORD x_row_buf = x_in;
		WLD_X_COORD x_out = x_in - 2;
		WLD_Y_COORD y_out = y_in - 1;

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
	}

	return 0;
}
