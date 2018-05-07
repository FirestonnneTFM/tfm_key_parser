#include "abc_file.hpp"
#include "method_info.hpp"

#define NEED_ARGUMENTS 0x01
#define NEED_ACTIVATION 0x02
#define NEED_REST 0x04
#define HAS_OPTIONAL 0x08
#define SET_DXNS 0x40
#define HAS_PARAM_NAMES 0x80

void method_info::load_header(abc_stream &stream) {
	uint32_t param_count;
	stream >> param_count;
	uint32_t index;	
	stream >> index;
	return_type = abc_file::get_multiname(index);
	param_type = Array<multiname_info *>(param_count);
	for (unsigned i = 0; i < param_count; ++i) {
		stream >> index;
		param_type[i] = abc_file::get_multiname(index);
	}
	stream >> index;
	str = abc_file::get_str(index);
	stream >> flags;
	if (flags & HAS_OPTIONAL) {
		uint32_t count;
		stream >> count;
		uint32_t dummy1;
		uint8_t dummy2;
		for (unsigned i = 0; i < count; ++i) {
			stream >> dummy1;
			stream >> dummy2;
		}
	}
	if (flags & HAS_PARAM_NAMES) {
		uint32_t dummy1;
		for (unsigned i = 0; i < param_count; ++i) {
			stream >> dummy1;
		}
	}
}

void method_info::load_body(abc_stream &stream) {
	stream >> max_stack;
	stream >> local_count;
	stream >> init_scope_depth;
	stream >> max_scope_depth;
	uint32_t count;
	stream >> count;
	code = Array<uint8_t>(count);
	for (unsigned i = 0; i < count; ++i) {
		stream >> code[i];
	}
	stream >> count;
	exceptions = Array<exception_info>(count);
	for (unsigned i = 0; i < count; ++i) {
		exceptions[i].load(stream);
	}
	stream >> count;
	traits = Array<traits_info>(count);
	for (unsigned i = 0; i < count; ++i) {
		traits[i].load(stream);
	}
}
