#include "abc_executer.hpp"
#include "abc_file.hpp"
#include "as3_op.hpp"
#include <string.h>

uint8_t abc_executer::step() {
	int32_t dummy;
	return step(&dummy);
}

uint8_t abc_executer::step(int32_t *ret) {
	uint8_t op;
	*this >> op;
	if (AS3_OP[op] == nullptr) {
		printf("opcode = 0x%02x\n", op);
		fatal("Unknown opcode");
	}
	size_t len = strlen(AS3_OP[op]);
	for (unsigned i = 0; i < len; ++i) {
		switch (AS3_OP[op][i]) {
		case 'u': {
			uint32_t dummy;
			*this >> dummy;
			*ret = dummy;
			break;
		}
		case 'b': {
			uint8_t dummy;
			*this >> dummy;
			*ret = dummy;
			break;
		}
		case 'h': {
			uint32_t dummy;
			*this >> dummy;
			*ret = (int16_t)dummy;
			break;
		}
		case 'i': {
			*ret = s24();
			break;
		}
		default:
			std::cout << "opcode param = " << AS3_OP[op][i] << std::endl;
			fatal("Unknown opcode param");
		}
	}
	return op;
}

void abc_executer::step_until(uint8_t op) {
	while (step() != op);
}

int32_t abc_executer::get_push() {
	uint8_t op;
	*this >> op;
	switch (op) {
	case OP_pushbyte: {
		uint8_t value;
		*this >> value;
		return value;
	}
	case OP_pushuint: {
		uint32_t index;
		*this >> index;
		return abc_file::get_uint(index);
	}
	case OP_pushint: {
		uint32_t index;
		*this >> index;
		return abc_file::get_int(index);
	}
	case OP_pushshort: {
		uint32_t value;
		*this >> value;
		return value;
	}
	default:
		std::cout << "Push is op 0x" << std::hex << +op << std::endl;
		fatal("Invalid push op");
	}
}

void abc_executer::rel_jmp(int32_t where) {
	ptr += where;
}
