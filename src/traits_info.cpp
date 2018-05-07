#include "traits_info.hpp"
#include "abc_file.hpp"

#define ATTR_Final 0x01
#define ATTR_Override 0x02
#define ATTR_Metadata 0x04

void traits_info::load(abc_stream &stream){
	has_loaded_value = false;
	uint32_t name_index;
	stream >> name_index;
	if (name_index == 0)
		fatal("Multiname index cannot be zero");
	name = abc_file::get_multiname(name_index);
	stream >> kind;
	uint8_t kind_high = kind >> 4;
	kind = kind & 0x0f;
	if (kind_high & ATTR_Metadata)
		fatal("Trait attribute metadata unsupported");
	uint32_t slot_id;
	switch (kind) {
	case Trait_Slot:
	case Trait_Const: {
		stream >> slot_id;
		stream >> vindex;
		stream >> vindex;
		if (vindex) {
			uint8_t dummy;
			stream >> dummy;
		}
		break;
	}
	case Trait_Class:
	case Trait_Method:
	case Trait_Getter:
	case Trait_Setter:
	case Trait_Function:
		stream >> slot_id;
		stream >> vindex;
		break;
	default:
		std::cout << "traits_info, kind = " << +kind << std::endl;
		fatal("Low bits of trait kind unknown");
	}
}
