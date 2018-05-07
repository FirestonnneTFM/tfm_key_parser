#pragma once

#include "common.hpp"
#include "abc_stream.hpp"
#include "abc_version.hpp"
#include "cpool_info.hpp"
#include "method_info.hpp"
#include "class_info.hpp"
#include "script_info.hpp"

class abc_file {
private:
	static abc_file *self;

	abc_version version;
	cpool_info constant_pool;
	void process(uint8_t *&);
	abc_file();
public:
	static abc_file *factory_from_bytes(uint8_t *);
	
	static char *get_str(uint32_t);
	static uint32_t get_uint(uint32_t);
	static int32_t get_int(uint32_t);
	static double get_double(uint32_t);
	static namespace_info *get_namespace(uint32_t);
	static multiname_info *get_multiname(uint32_t);
	static Array<namespace_info*> get_namespace_set(uint32_t);
	static method_info *get_method(uint32_t);
	static class_info *get_class(uint32_t);
	static class_info *get_class_by_name(uint32_t);

	Array<method_info> methods;
	Array<class_info> classes;
	Array<script_info> scripts;
};
