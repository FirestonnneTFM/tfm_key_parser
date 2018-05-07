#pragma once

#include "abc_stream.hpp"

class exception_info {
public:
	uint32_t from;
	uint32_t to;
	uint32_t target;
	char *exc_type;
	char *var_name;
	void load(abc_stream &stream);
};
