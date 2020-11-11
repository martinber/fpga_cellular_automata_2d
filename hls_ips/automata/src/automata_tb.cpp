#include <iostream>
#include <cstdlib>
#include <vector>
#include "automata.h"

int automata_sw(int a, int b, int c);
int unit_test(int a, int b, int c);
int random_unit_test();

int main() {
	int a, b, c;
	int test_res;
	srand(3456);
	a = 5;
	b = 3;
	c = 7;
	std::cout << "Test vector: a: " << a << ", b: " << b << ", c: " << c << std::endl;
	test_res = unit_test(a, b, c);
	if (test_res) {
		std::cout << "Test run failed!" << std::endl;
		return test_res;
	} else {
		std::cout << "Test run OK!" << std::endl;
	}

	test_res = random_unit_test();
	if (test_res) {
		std::cout << "Test run failed!" << std::endl;
		return test_res;
	} else {
		std::cout << "Test run OK!" << std::endl;
	}

	for (int i = 0; i < 10; i++) {
		std::cout << "Test run: " << i << std::endl;
		test_res = random_unit_test();
		if (test_res) {
			std::cout << "Test run failed!" << std::endl;
			return test_res;
		} else {
			std::cout << "Test run OK!" << std::endl;
		}
	}
	return test_res;
}

int unit_test(int a, int b, int c) {
	int res_original;
	int res_fpga;
	int test_res = 0;
//	Call original function
	res_original = automata_sw(a, b, c);
//	Call FPGA optimized function
	res_fpga = automata_hw(a, b, c);
//	Compare results
	if (res_original != res_fpga) {
		std::cout << "Error! Expected: " << res_original << " got: " << res_fpga
				<< std::endl;
		test_res = 1;
	}
	return test_res;
}

int random_unit_test() {
	int a, b, c;
//	Generate random input
	a = rand() % 1024;
	b = rand() % 1024;
	c = rand() % 1024;
	std::cout << "Test vector: a: " << a << ", b: " << b << ", c: " << c << std::endl;
	return unit_test(a, b, c);
}

int automata_sw(int a, int b, int c) {
	return a * b + c;
}
