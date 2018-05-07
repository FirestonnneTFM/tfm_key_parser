#pragma once

#include "common.hpp"
#include "abc_stream.hpp"
#include "exception_info.hpp"
#include "traits_info.hpp"

class method_info {
public:
	Array<multiname_info *> param_type;
	multiname_info *return_type;
	char *str;
	uint8_t flags;
	uint32_t max_stack;
	uint32_t local_count;
	uint32_t init_scope_depth;
	uint32_t max_scope_depth;
	Array<uint8_t> code;
	Array<exception_info> exceptions;
	Array<traits_info> traits;
	
	void load_header(abc_stream &stream);
	void load_body(abc_stream &stream);
};
