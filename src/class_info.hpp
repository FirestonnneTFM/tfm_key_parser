#pragma once

#include "multiname_info.hpp"
#include "method_info.hpp"
#include "traits_info.hpp"
#include <map>

class class_info {
private:
	bool static_init_already_done;
	std::map<multiname_info*, traits_info*> ordered_trait;
public:
	traits_info *get_trait_by_name(multiname_info *);
	
	multiname_info *name;
	multiname_info *super_name;
	uint8_t flags;
	Array<multiname_info*> interfaces;
	method_info *ctor;
	Array<traits_info> traits;
	method_info *static_ctor;
	Array<traits_info> static_traits;
	void load_inst(abc_stream &stream);
	void load_static(abc_stream &stream);
	void do_static_init();
};
