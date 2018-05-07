#include "script_info.hpp"
#include "abc_file.hpp"

void script_info::load(abc_stream &stream) {
	uint32_t index;
	stream >> index;
	init = abc_file::get_method(index);
	uint32_t count;
	stream >> count;
	traits = Array<traits_info>(count);
	for (unsigned i = 0; i < count; ++i) {
		traits[i].load(stream);
	}
}
