#include "common.hpp"
#include "swf_file.hpp"
#include "abc_executer.hpp"
#include "as3_op.hpp"
#include <string.h>
#include "swf_tag/DefineBinaryData.hpp"
#include "swf_tag/SymbolClass.hpp"

static char *COP;


class executer : public abc_executer {
public:
	executer(uint8_t *dat) : abc_executer(dat) {}
	char call_prop(class_info *);
	void pattern_1();
};

char executer::call_prop(class_info *c) {
	uint8_t op;
	*this >> op;
	if (op != OP_callproperty) {
		undo();
		return '\0';
	}
	uint32_t index, arg_count;
	*this >> index;
	*this >> arg_count;
	traits_info *trait = c->get_trait_by_name(abc_file::get_multiname(index));
	method_info *prop = abc_file::get_method(trait->vindex);
	executer e(prop->code.begin());
	assert(e.step() == OP_getlocal_0);
	assert(e.step() == OP_pushscope);
	assert(e.step() == OP_getlex);
	return COP[e.get_push()];
}

void executer::pattern_1() {
	for ( ; ; ) {
		if (step() == OP_getlocal_1
			&& step() == OP_getlocal_0
			&& step() == OP_getlocal_0) {
			break;
		}
	}
}

static char *make_global(char *old) {
	size_t len = strlen(old);
	++len;
	char *buf = new char[len];
	memcpy(buf, old, len);
	return buf;
}

void go(char **bufs) {
	method_info *info = abc_file::get_class(0)->static_ctor;
	executer exec(info->code.begin());
	exec.step_until(OP_findproperty);
	uint32_t op;
	exec >> op;
	assert(op == OP_pushstring);
	uint32_t index;
	exec >> index;
	COP = abc_file::get_str(index);

	info = abc_file::get_class(0)->ctor;
	exec = executer(info->code.begin());
	for (int i = 0; i < 12; ++i) {
		exec.pattern_1();
		char buf[50];
		int j = 0;
		buf[j] = exec.call_prop(abc_file::get_class(0));
		++j;
		do {
			// usually is OP_getlocal_0
			exec.step();
			buf[j] = exec.call_prop(abc_file::get_class(0));
			++j;
		} while (exec.step() == OP_add);
		bufs[i] = make_global(buf);
	}
}

static inline char *prune(char *buf) {
	while (*buf != '_')
		++buf;
	return buf + 1;
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
		swf_tag tags[12];
		swf_tag_DefineBinaryData btags[12];
		swf->get_tags(87, tags, 12);
		for (int i = 0; i < 12; ++i) {
			btags[i] = swf_tag_DefineBinaryData(tags + i);
			if (btags[i].ch_id != i + 1)
				fatal("Tag and position mismatch");
		}
		swf->get_tags(76, tags, 1);
		swf_tag_SymbolClass sym(tags);
		char *order[12];
		go(order);
		char *unorder[12];
		for (int i = 0; i < 12; ++i) {
			unorder[sym.ch_ids[i] - 1] = prune(sym.names[i]);
		}
		FILE *out_file = fopen(argv[2], "wb");
		for (int i = 0; i < 12; ++i) {
			for (int j = 0; j < 12; ++j) {
				if (strcmp(order[i], unorder[j]) == 0) {
					fwrite(btags[j].binary_data, 1, btags[j].length, out_file);
				}
			}
		}
		fclose(out_file);
		for (int i = 0; i < 12; ++i) {
			delete [] order[i];
		}
		delete swf;
		return 0;
	} catch (fatal_error e) {
		e.print();
	} catch (const char *e) {
		std::cout << "DEPRECATED -- instead of throwing string, use macro"<< std::endl;
		fprintf(stderr, "Fatal: %s\n", e);
		return 1;
	}
}
