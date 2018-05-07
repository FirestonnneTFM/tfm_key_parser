#pragma once

#include "common.hpp"
#include "abc_stream.hpp"

class abc_executer : public abc_stream {
public:
	abc_executer(uint8_t *dat) : abc_stream(dat) {}
	uint8_t step();
	uint8_t step(int32_t *);
	void step_until(uint8_t);
	int32_t get_push();
	void rel_jmp(int32_t);
};
