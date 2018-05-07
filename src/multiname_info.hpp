#pragma once

#include "abc_stream.hpp"
#include "namespace_info.hpp"

class class_info;

class multiname_info {
public:
	uint8_t kind;
	char *ns_str;
	char *str;
	class_info *target;
	Array<namespace_info*> mns;
	void load(abc_stream &stream);
};
