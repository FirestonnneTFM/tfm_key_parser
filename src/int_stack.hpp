#pragma once

#include "common.hpp"

class int_stack {
private:
	int32_t *ptr;
	int32_t *arr;
public:
	int_stack(int);
	void push(int32_t);
	int32_t pop();
	int32_t top() {
		return ptr[-1];
	}
	int count() {
		return ptr - arr;
	}
};
