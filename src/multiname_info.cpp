#include "multiname_info.hpp"
#include "abc_file.hpp"

#define CONSTANT_QName 0x07
#define CONSTANT_QNameA 0x0D
#define CONSTANT_RTQName 0x0F
#define CONSTANT_RTQNameA 0x10
#define CONSTANT_RTQNameL 0x11
#define CONSTANT_RTQNameLA 0x12
#define CONSTANT_Multiname 0x09
#define CONSTANT_MultinameA 0x0E
#define CONSTANT_MultinameL 0x1B
#define CONSTANT_MultinameLA 0x1C
#define CONSTANT_Typename 0x1D

void multiname_info::load(abc_stream &stream) {
	target = nullptr;
	stream >> kind;
	uint32_t index;
	switch (kind) {
	case CONSTANT_QName:
	case CONSTANT_QNameA:
		stream >> index;
		ns_str = abc_file::get_namespace(index)->str;
		stream >> index;
		str = abc_file::get_str(index);
		break;
	case CONSTANT_RTQName:
	case CONSTANT_RTQNameA:
		ns_str = abc_file::get_str(0);
		stream >> index;
		str = abc_file::get_str(index);
		break;
	case CONSTANT_RTQNameL:
	case CONSTANT_RTQNameLA:
		ns_str = abc_file::get_str(0);
		str = abc_file::get_str(0);
		break;
	case CONSTANT_Multiname:
	case CONSTANT_MultinameA:
		ns_str = abc_file::get_str(0);
		stream >> index;
		str = abc_file::get_str(index);
		stream >> index;
		mns = abc_file::get_namespace_set(index);
		break;
	case CONSTANT_MultinameL:
	case CONSTANT_MultinameLA:
		stream >> index;
		str = abc_file::get_str(0);
		ns_str = abc_file::get_str(0);
		mns = abc_file::get_namespace_set(index);
		break;
	case CONSTANT_Typename:
		stream >> index;
		str = abc_file::get_multiname(index)->str;
		ns_str = abc_file::get_str(0);
		stream >> index;
		for (unsigned i = 0; i < index; ++i) {
			uint32_t dummy;
			stream >> dummy;
		}
		break;
	default:
		std::cout << "multiname kind = " << +kind << std::endl;
		fatal("Illegal multiname kind");
	}
}
