#pragma once

#include "multiname_info.hpp"

class multiname_info;

#define Trait_Slot 0
#define Trait_Method 1
#define Trait_Getter 2
#define Trait_Setter 3
#define Trait_Class 4
#define Trait_Function 5
#define Trait_Const 6

class traits_info {
public:
	multiname_info *name;
	uint8_t kind;
	uint32_t vindex;
	bool has_loaded_value;
	int32_t loaded_value;
	
	void load(abc_stream &stream);
};
