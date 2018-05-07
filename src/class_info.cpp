#include "class_info.hpp"
#include "abc_file.hpp"
#include "int_stack.hpp"
#include "abc_executer.hpp"
#include "as3_op.hpp"

#define CONSTANT_ClassSealed 0x01
#define CONSTANT_ClassFinal 0x02
#define CONSTANT_ClassInterface 0x04
#define CONSTANT_ClassProtectedNs 0x08

void class_info::load_inst(abc_stream &stream) {
	uint32_t index, count;
	stream >> index;
	name = abc_file::get_multiname(index);
	if (name->target != nullptr) {
		std::cout << name->str << " is duplicated" << std::endl;
	} else {
		name->target = this;
	}
	stream >> index;
	super_name = abc_file::get_multiname(index);
	stream >> flags;
	if (flags & CONSTANT_ClassProtectedNs) {
		uint32_t dummy;
		stream >> dummy;
	}
	stream >> count;
	interfaces = Array<multiname_info*>(count);
	for (unsigned i = 0; i < count; ++i) {
		stream >> index;
		interfaces[i] = abc_file::get_multiname(index);
	}
	stream >> index;
	ctor = abc_file::get_method(index);
	stream >> count;
	traits = Array<traits_info>(count);
	for (unsigned i = 0; i < count; ++i) {
		traits[i].load(stream);
		ordered_trait.insert({ traits[i].name, traits.begin() + i });
	}
}

void class_info::load_static(abc_stream &stream) {
	static_init_already_done = false;
	uint32_t index, count;
	stream >> index;
	static_ctor = abc_file::get_method(index);
	stream >> count;
	static_traits = Array<traits_info>(count);
	for (unsigned i = 0; i < count; ++i) {
		static_traits[i].load(stream);
		ordered_trait.insert({ static_traits[i].name, static_traits.begin() + i });
	}
}

void class_info::do_static_init() {
	if (static_init_already_done)
		return;
	static_init_already_done = true;
	abc_executer exec(static_ctor->code.begin());
	bool ret = true;
	int_stack stack(static_ctor->max_stack);
	while (ret) {
		int32_t param;
		uint8_t op = exec.step(&param);
		switch (op) {
		case OP_returnvoid:
			ret = false;
			break;
		case OP_pushint:
		case OP_pushuint:
		case OP_pushbyte:
		case OP_pushshort:
			stack.push(param);
			break;
		case OP_pushfalse:
		case OP_pushstring:
		case OP_getproperty:
			stack.push(0);
			break;
		case OP_pushtrue:
			stack.push(1);
			break;
		case OP_not:
			if (stack.pop())
				stack.push(0);
			else
				stack.push(1);
			break;
		case OP_equals:
			if (stack.pop() == stack.pop())
				stack.push(1);
			else
				stack.push(0);
			break;
		case OP_dup:
			stack.push(stack.top());
			break;
		case OP_pop:
			stack.pop();
			break;
		case OP_add:
			stack.push(stack.pop() + stack.pop());
			break;
		case OP_multiply:
			stack.push(stack.pop() * stack.pop());
			break;
		case OP_divide: {
			int p = stack.pop();
			int q = stack.pop();
			if (p == 0 || q == 0)
				stack.push(0);
			else
				stack.push(p / q);
			break;
		}
		case OP_setproperty:
		case OP_initproperty: {
			traits_info *t = get_trait_by_name(abc_file::get_multiname(param));
			t->has_loaded_value = true;
			t->loaded_value = stack.pop();
			break;
		}
		case OP_jump:
			exec.rel_jmp(param);
			break;
		case OP_iffalse:
			if (stack.pop() == 0)
				exec.rel_jmp(param);
			break;
		case OP_getlocal_0:
		case OP_pushscope:
		case OP_findproperty:
		case OP_findpropstrict:
		case OP_getlex:
		case OP_callproperty:
			// do nothing
			break;
		default:
			std::cout << "op = 0x" << std::hex << +op << std::endl;
			fatal("static init not handling op");
		}
	}
}

traits_info *class_info::get_trait_by_name(multiname_info *key) {
	auto search = ordered_trait.find(key);
	if(search != ordered_trait.end()) {
		return search->second;
	} else {
		std::cout << "name = " << key->str << std::endl;
		fatal("Could not find trait by name");
	}
}
