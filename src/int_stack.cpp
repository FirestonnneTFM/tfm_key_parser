#include "int_stack.hpp"

int_stack::int_stack(int n) {
	if (n != 0) {
		arr = new int32_t[n];
	} else {
		arr = nullptr;
	}
	ptr = arr;
}

void int_stack::push(int32_t value) {
	*ptr = value;
	++ptr;
}

int32_t int_stack::pop() {
	--ptr;
	return *ptr;
}
