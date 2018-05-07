#include "cpool_info.hpp"
#include "abc_file.hpp"
#include <math.h>

cpool_info::cpool_info() {}

void cpool_info::load(abc_stream &stream) {
	uint32_t count;
	stream >> count;
	count = count ? count : 1;
	ints = Array<int32_t>(count);
	ints[0] = 0;
	for (unsigned i = 1; i < count; ++i) {
		stream >> ints[i];
	}
	stream >> count;
	count = count ? count : 1;
	uints = Array<uint32_t>(count);
	uints[0] = 0;
	for (unsigned i = 1; i < count; ++i) {
		stream >> uints[i];
	}
	stream >> count;
	count = count ? count : 1;
	doubles = Array<double>(count);
	doubles[0] = NAN;
	for (unsigned i = 1; i < count; ++i) {
		stream >> doubles[i];
	}
	stream >> count;
	count = count ? count : 1;
	strings = Array<char*>(count);
	strings[0] = new char[1];
	strings[0][0] = '\0';
	for (unsigned i = 1; i < count; ++i) {
		stream >> strings[i];
	}
	stream >> count;
	count = count ? count : 1;
	namespaces = Array<namespace_info>(count);
	namespaces[0].kind = 0;
	namespaces[0].str = NULL;
	for (unsigned i = 1; i < count; ++i) {
		stream >> namespaces[i].kind;
		uint32_t name_index;
		stream >> name_index;
		namespaces[i].str = abc_file::get_str(name_index);
	}
	stream >> count;
	count = count ? count : 1;
	namespace_sets = Array<Array<namespace_info*>>(count);
	namespace_sets[0] = Array<namespace_info*>(0);
	for (unsigned i = 1; i < count; ++i) {
		uint32_t n;
		stream >> n;
		namespace_sets[i] = Array<namespace_info*>(n);
		for (unsigned j = 0; j < n; ++j) {
			uint32_t index;
			stream >> index;
			if (index == 0) {
				fatal("namespace set contained 0 (namespace *ANY)");
			}
			namespace_sets[i][j] = abc_file::get_namespace(index);
		}
	}
	stream >> count;
	count = count ? count : 1;
	multinames = Array<multiname_info>(count);
	multinames[0].kind = 0;
	multinames[0].ns_str = abc_file::get_str(0);
	multinames[0].str = abc_file::get_str(0);
	for (unsigned i = 1; i < count; ++i) {
		multinames[i].load(stream);
	}
}
