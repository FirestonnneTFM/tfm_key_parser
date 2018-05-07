#pragma once

#include "method_info.hpp"
#include "traits_info.hpp"

class script_info {
public:
	method_info *init;
	Array<traits_info> traits;

	void load(abc_stream &stream);
};
