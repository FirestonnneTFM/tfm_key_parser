#pragma once
#include <stdint.h>

class abc_version {
public:
	uint16_t minor;
	uint16_t major;
	bool suspicious();
	void print();
};
