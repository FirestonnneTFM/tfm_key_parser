#pragma once

#include "abc_stream.hpp"
#include "multiname_info.hpp"
#include "namespace_info.hpp"

class cpool_info {
public:
	Array<int32_t> ints;
	Array<uint32_t> uints;
	Array<double> doubles;
	Array<char *> strings;
	Array<namespace_info> namespaces;
	Array<Array<namespace_info*>> namespace_sets;
	Array<multiname_info> multinames;
	
	void load(abc_stream &);
	cpool_info();
};
