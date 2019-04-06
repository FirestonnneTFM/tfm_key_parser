#include "common.hpp"
#include "swf_file.hpp"
#include "abc_executer.hpp"
#include "as3_op.hpp"
#include <string.h>

class keys {
public:
	uint16_t version;
	int32_t login;
	uint8_t hash[20];
	char handshake[20];
	keys();
	void print();
	void write_to(FILE *f);
};

keys::keys() {
	version = 0;
	login = 0;
	for (unsigned i = 0; i < 20; ++i) {
		hash[i] = 0;
		handshake[i] = 0;
	}
}

void keys::print() {
	std::cout << "version = " << version << std::endl;
	std::cout << "login = " << login << std::endl;
	std::cout << "handshake = " << handshake << std::endl;
	std::cout << "hash =";
	for (unsigned i = 0; i < 20; ++i) {
		printf(" %02x", hash[i]);
	}
	std::cout << std::endl;
}

void keys::write_to(FILE *f) {
	fwrite(&version, 2, 1, f);
	fwrite(&login, 4, 1, f);
	fwrite(hash, 1, 20, f);
	fwrite(handshake, 1, 20, f);
}

static keys key;

class executer : public abc_executer {
public:
	executer(uint8_t *dat) : abc_executer(dat) {}
	executer(method_info *m) : abc_executer(m->code.begin()) {}
	int32_t get_lex_int();
	bool get_lex_bool();
	char *get_lex_str();
};

int32_t executer::get_lex_int() {
	uint32_t index, arg_count;
	*this >> index;
	if (strlen(abc_file::get_multiname(index)->ns_str))
		fatal("class is out of our namespace");
	class_info *c = abc_file::get_class_by_name(index);
	c->do_static_init();
	assert(next() == OP_callproperty);
	*this >> index;
	*this >> arg_count;
	assert(arg_count == 0);
	traits_info *tm = c->get_trait_by_name(abc_file::get_multiname(index));
	method_info *m = abc_file::get_method(tm->vindex);
	executer exec(m);
	assert(exec.step() == OP_getlocal_0);
	assert(exec.step() == OP_pushscope);
	int32_t a = exec.get_push();
	int32_t b = exec.get_push();
	assert(exec.step() == OP_add);
	assert(exec.step() == OP_returnvalue);
	return a + b;
}

bool executer::get_lex_bool() {
	uint32_t index;
	*this >> index;
	if (strlen(abc_file::get_multiname(index)->ns_str))
		fatal("class is out of our namespace");
	class_info *c = abc_file::get_class_by_name(index);
	c->do_static_init();
	assert(next() == OP_getproperty);
	*this >> index;
	traits_info *trait = c->get_trait_by_name(abc_file::get_multiname(index));
	if (! trait->has_loaded_value)
		fatal("The static trait does not have a loaded value");
	return trait->loaded_value != 0;
}

char *executer::get_lex_str() {
	uint32_t index;
	*this >> index;
	if (strlen(abc_file::get_multiname(index)->ns_str))
		return abc_file::get_str(0);
	class_info *c = abc_file::get_class_by_name(index);
	c->do_static_init();
	assert(next() == OP_getproperty);
	*this >> index;
	traits_info *trait = c->get_trait_by_name(abc_file::get_multiname(index));
	return abc_file::get_str(trait->vindex);
}

static uint8_t *key_hash_output = nullptr;

static void get_hash_quad(executer *exec) {
	for (int i = 0; i < 4; ++i, ++key_hash_output) {
		assert(exec->next() == OP_getlex);
		*key_hash_output = (uint8_t)exec->get_lex_int();
	}
}

static void do_mark0(method_info *m) {
	executer exec(m);
	exec.step_until(OP_iftrue);
	exec.step_until(OP_getlocal);
	get_hash_quad(&exec);
}

static void do_mark1(method_info *m) {
	executer exec(m);
	exec.step_until(OP_pushnull);
	exec.step_until(OP_pushnull);
	exec.step_until(OP_pushnull);
	exec.step_until(OP_getlocal_3);
	get_hash_quad(&exec);
}

static void do_mark2(method_info *m) {
	executer exec(m);
	exec.step_until(OP_getlocal_1);
	get_hash_quad(&exec);
}

static void do_mark3(method_info *m) {
	executer exec(m);
	exec.step_until(OP_getlocal_1);
	get_hash_quad(&exec);
}

static void do_mark4(method_info *m) {
	executer exec(m);
	exec.step_until(OP_getlocal_3);
	get_hash_quad(&exec);
}

static bool is_mark1(method_info *m) {
	if (m->param_type.length() != 1)
		return false;
	if (m->code.length() < 30)
		return false;
	if (! streq(m->param_type[0]->str, "Event"))
		return false;
	uint8_t expected[9] = { 0xd0, 0x30, 0x57, 0x2a, 0xd6, 0x30, 0x65, 0x01, 0x20 };
	for (unsigned i = 0; i < 9; ++i) {
		if (m->code[i] != expected[i])
			return false;
	}
	return true;
}

static void find_hash() {
	key_hash_output = key.hash;
	method_info *mark[5];
	class_info *c = abc_file::get_class(0);
	mark[0] = c->ctor;
	mark[1] = nullptr;
	mark[2] = nullptr;
	mark[3] = nullptr;
	mark[4] = nullptr;
	for (auto trait : c->traits) {
		if (trait.kind == Trait_Method) {
			method_info *method = abc_file::get_method(trait.vindex);
			if (is_mark1(method)) {
				if (mark[1])
					fatal("mark1 not unique");
				mark[1] = method;
			}
			if (method->param_type.length() == 0
				&& streq(method->return_type->str, "void")) {
				if (streq(trait.name->str, "Initialisation")) {
					if (mark[3])
						fatal("mark3 not unique");
					mark[3] = method;
				}
			}
			if (method->code.length() > 100 && method->code.length() < 220) {
				executer exec(method);
				if (exec.step() != OP_getlocal_0)
					continue;
				if (exec.step() != OP_pushscope)
					continue;
				if (exec.step() == OP_findpropstrict) {
					if (mark[2])
						fatal("mark2 not unique");
					mark[2] = method;
				}
				if (method->max_stack == 6 && method->local_count == 4 && method->param_type.length() == 0) {
					if (mark[4])
						fatal("mark4 not unique");
					mark[4] = method;
				}
			}
		}
	}
	for (unsigned i = 0; i < 5; ++i) {
		if (mark[i] == nullptr) {
			std::cout << "mark" << i << " is null" << std::endl;
			fatal("mark not found");
		}
	}
	do_mark0(mark[0]);
	do_mark1(mark[1]);
	do_mark2(mark[2]);
	do_mark3(mark[3]);
	do_mark4(mark[4]);
}

static void find_handshake() {
	class_info *c = abc_file::get_class(0);
	c->do_static_init();
	uint8_t *target = nullptr;
	for (auto trait : c->traits) {
		if (trait.kind == Trait_Method) {
			method_info *method = abc_file::get_method(trait.vindex);
			if (method->param_type.length() == 1 && method->code.length() > 50
				&& streq(method->param_type[0]->str, "Event")) {
				executer exec(method);
				if (exec.step() != OP_getlocal_0)
					continue;
				if (exec.step() != OP_pushscope)
					continue;
				if (exec.step() != OP_pushnull)
					continue;
				if (exec.next() != OP_coerce)
					continue;
				uint32_t obj;
				exec >> obj;
				if (! streq(abc_file::get_multiname(obj)->str, "Font"))
					continue;
				if (target)
					fatal("Handshake method not unqiue");
				else
					target = method->code.begin();
			}
		}
	}
	if (target == nullptr)
		fatal("Could not find suitable handshake method");
	executer exec(target);
	exec.step_until(OP_inclocal_i);
	exec.step_until(OP_setlocal);
	exec.step_until(OP_getproperty);
	exec.step_until(OP_getproperty);
	exec.step_until(OP_getproperty);
	char *str = key.handshake;
	for (int count = 0; count < 2 || exec.step() == OP_add; ++count) {
		assert(exec.next() == OP_getlex);
		char *ret = exec.get_lex_str();
		size_t len = strlen(ret);
		for (unsigned i = 0; i < len; ++i, ++str) {
			*str = ret[i];
		}
	}
}

static void find_login() {
	class_info *c = abc_file::get_class(0);
	c->do_static_init();
	method_info *target = nullptr;
	for (auto trait : c->traits) {
		if (trait.kind == Trait_Method) {
			method_info *method = abc_file::get_method(trait.vindex);
			if (method->param_type.length() == 0
				&& streq(method->return_type->str, "int")) {
				target = method;
			}
		}
	}
	if (target == nullptr)
		fatal("Could not find key login");
	executer exec(target);
	assert(exec.step() == OP_getlocal_0);
	assert(exec.step() == OP_pushscope);
	assert(exec.step() == OP_getlocal_0);
	assert(exec.step() == OP_getproperty);
	assert(exec.step() == OP_convert_i);
	while (exec.step() == OP_setlocal_1) {
		assert(exec.step() == OP_getlocal_1);
		uint8_t op = exec.next();
		if (op == OP_returnvalue)
			break;
		assert(op == OP_getlex);
		int32_t a = exec.get_lex_int();
		op = exec.next();
		if (op == OP_bitxor) {
			key.login ^= a;
			continue;
		}
		assert(op == OP_getlex);
 		int32_t b = exec.get_lex_int();
		assert(exec.step() == OP_lshift);
		assert(exec.step() == OP_bitxor);
		key.login ^= a << b;
	}
}

static void find_version() {
	class_info *c = abc_file::get_class(0);
	executer exec(c->ctor);
	exec.step_until(OP_constructsuper);
	exec.step_until(OP_setproperty);
	assert(exec.next() == OP_getlex);
	if (! exec.get_lex_bool()) {
		fatal("(unimplemented) The key version is burried deeper");
	}
	key.version = (uint16_t)abc_file::get_double(c->traits[0].vindex);
}

int main(int argc, char **argv) {
	std::ios_base::sync_with_stdio(true);
	try {
		if (argc < 2) {
			fatal("Needs input file name");
		}
		if (argc < 3) {
			fatal("Needs output file name");
		}
		init_as3_op();
		swf_file *swf = new swf_file(argv[1]);
		find_version();
		find_login();
		find_handshake();
		find_hash();
		key.print();
		FILE *out_file = fopen(argv[2], "wb");
		key.write_to(out_file);
		fclose(out_file);
		delete swf;
		return 0;
	} catch (fatal_error e) {
		e.print();
	} catch (const char *e) {
		std::cout << "DEPRECATED -- instead of throwing string, use macro" << std::endl;
		fprintf(stderr, "Fatal: %s\n", e);
		return 1;
	}
}
