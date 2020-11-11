#include "automata.h"

int automata_hw(World *w_in, World *w_out) {
	for (WLD_COORD x = 0; x < WLD_W; x++) {
		for (WLD_COORD y = 0; y < WLD_H; y++) {
			wld_set(w_out, x, y, wld_get(w_in, x, y));
		}
	}
	return 0;
}
