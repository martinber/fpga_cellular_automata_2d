#include "automata.h"

int automata_hw(hls::stream<CELL> &in_stream, hls::stream<CELL> &out_stream) {
#pragma HLS INTERFACE axis register both port=in_stream
#pragma HLS INTERFACE axis register both port=out_stream
#pragma HLS INTERFACE s_axilite port=return

	// Buffer with the last two rows
	CELL row_buf[2][WLD_W];

	// Buffer with the 3x3 neighborhood
	CELL neigh_buf[3][3]; // Not necessary to init

	// Currently read value and coordinates
	CELL curr_in = false;

	// Current position in row_buf
	WLD_X_COORD x_row_buf = 0;

	// Current position in output image
	WLD_X_COORD x_out = 0;
	WLD_X_COORD y_out = 0;

    // TODO: Use two loops
	main_loop: for (unsigned int i = 0; i < WLD_W * (WLD_H + 1) + 2; i++) {
#pragma HLS PIPELINE
		if (i < WLD_W * WLD_H) {
			curr_in = in_stream.read();
		}

		// Operate in neighborhood

		ap_uint<4> neighs = neigh_buf[0][0] + neigh_buf[0][1] + neigh_buf[0][2]
					      + neigh_buf[1][0]                   + neigh_buf[1][2]
					      + neigh_buf[2][0] + neigh_buf[2][1] + neigh_buf[2][2];

		CELL out;

		if (i > WLD_W + 1) {
			// If the input data reached the neigh_buf

			if (x_out != 0 && x_out != WLD_W - 1
					&& y_out != 0 && y_out != WLD_H - 1) {
				// If it is not in margin

				if (neigh_buf[1][1]) {
					// If the cell was alive

					out = neighs == 2 || neighs == 3;
				} else {
					// If the cell was dead

					out = neighs == 3;
				}
			} else {
				// If it is in margin

				out = 0; // Write margins as zero
			}

			out_stream.write(out);
		}


		// Update neigh_buf

		shift_neigh: for (int i = 0; i < 2; i++) {
#pragma HLS unroll
			neigh_buf[i][0] = neigh_buf[i+1][0];
			neigh_buf[i][1] = neigh_buf[i+1][1];
			neigh_buf[i][2] = neigh_buf[i+1][2];
		}

		// Temporary variables, because we don't want to read from row_buf too many times

		CELL row_buf_0 = row_buf[0][x_row_buf];
		CELL row_buf_1 = row_buf[1][x_row_buf];

		neigh_buf[2][0] = row_buf_0;
		neigh_buf[2][1] = row_buf_1;
		neigh_buf[2][2] = curr_in;

		// Update row_buf

		row_buf[0][x_row_buf] = row_buf_1;
		row_buf[1][x_row_buf] = curr_in;

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

