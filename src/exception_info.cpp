#include "exception_info.hpp"
#include "abc_file.hpp"

void exception_info::load(abc_stream &stream) {
	stream >> from;
	stream >> to;
	stream >> target;
	uint32_t str_index;
	stream >> str_index;
	exc_type = abc_file::get_str(str_index);
	stream >> str_index;
	var_name = abc_file::get_str(str_index);
}
