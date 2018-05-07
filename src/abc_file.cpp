#include "abc_file.hpp"
#include "abc_version.hpp"
#include <iostream>
#include <string.h>

#include "abc_file.hpp"

abc_file *abc_file::self = nullptr;

abc_file *abc_file::factory_from_bytes(uint8_t *data) {
	abc_file *abc = new abc_file();
	abc->process(data);
	return abc;
}

abc_file::abc_file() {
	self = this;
}

char *abc_file::get_str(uint32_t index) {
	return self->constant_pool.strings[index];
}

uint32_t abc_file::get_uint(uint32_t index) {
	return self->constant_pool.uints[index];
}

int32_t abc_file::get_int(uint32_t index) {
	return self->constant_pool.ints[index];
}

double abc_file::get_double(uint32_t index) {
	return self->constant_pool.doubles[index];
}

multiname_info *abc_file::get_multiname(uint32_t index) {
	return self->constant_pool.multinames.begin() + index;
}

namespace_info *abc_file::get_namespace(uint32_t index) {
	return self->constant_pool.namespaces.begin() + index;
}

Array<namespace_info*> abc_file::get_namespace_set(uint32_t index) {
	return self->constant_pool.namespace_sets[index];
}

method_info *abc_file::get_method(uint32_t index) {
	return self->methods.begin() + index;
}

class_info *abc_file::get_class(uint32_t index) {
	return self->classes.begin() + index;
}

class_info *abc_file::get_class_by_name(uint32_t index) {
	return self->constant_pool.multinames[index].target;
}

void abc_file::process(uint8_t *&data) {
	data += 4;
	data += strlen((char*)data) + 1;
	abc_stream stream(data);
	
	stream >> version.minor;
	stream >> version.major;
	
	if (version.suspicious()) {
		version.print();
		fatal("Suspicious abc version");
	}
	constant_pool.load(stream);
	uint32_t count;
	stream >> count;
	methods = Array<method_info>(count);
	for (unsigned i = 0; i < count; ++i) {
		methods[i].load_header(stream);
	}
	stream >> count;
	for (unsigned i = 0; i < count; ++i) {
		uint32_t dummy1, dummy2, len;
		stream >> dummy1;
		stream >> len;
		for (unsigned j = 0; i < len; ++j) {
			stream >> dummy1;
			stream >> dummy2;
		}
	}
	stream >> count;
	classes = Array<class_info>(count);
	for (unsigned i = 0; i < count; ++i) {
		classes[i].load_inst(stream);
	}
	for (unsigned i = 0; i < count; ++i) {
		classes[i].load_static(stream);
	}
	stream >> count;
	scripts = Array<script_info>(count);
	for (unsigned i = 0; i < count; ++i) {
		scripts[i].load(stream);
	}
	stream >> count;
	for (unsigned i = 0; i < count; ++i) {
		uint32_t which;
		stream >> which;
		methods[which].load_body(stream);
	}
}
