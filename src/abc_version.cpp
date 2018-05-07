#include "abc_version.hpp"
#include <iostream>

void abc_version::print() {
	std::cout << "abc_version: " << major << '.' << minor << std::endl;
}

bool abc_version::suspicious() {
	return major != 46 || minor != 16;
}
