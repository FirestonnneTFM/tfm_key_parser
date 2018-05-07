#pragma once

#include <stdint.h>
#include <stdio.h>
#include <iostream>

#define UNUSED(x)((void)(x))

class fatal_error {
public:
	int line;
	const char *fname;
	const char *message;
	fatal_error(const char *message0, const char *fname0, int line0)
		: line(line0), fname(fname0), message(message0) {}
	void print() {
		std::cout << "Fatal: " << message;
		std::cout << " (" << fname << ':' << line << ')' << std::endl;
	}
};

#define fatal(msg)(throw fatal_error(msg, __FILE__, __LINE__))
#define assert(what) do {											\
		if (! (what))												\
			throw fatal_error("Assert failed", __FILE__, __LINE__);	\
	} while(0);

#define bool_str(v)((v) ? "true" : "false")
#define streq(a, b)(strcmp(a, b) == 0)

template <typename T>
class Array {
private:
	uint32_t count;
	T *arr;
public:
	Array() {
		count = 0;
		arr = nullptr;
	}
	Array(uint32_t n) {
		count = n;
		if (n == 0)
			arr = nullptr;
		else
			arr = new T[n];
	}
	T *begin() {
		return arr;
	}
	T *end() {
		return arr + count;
	}
	uint32_t length() {
		return count;
	}
	T &operator[](int index) {
		return arr[index];
	}
};

template <typename T>
static inline T conv_copy(uint8_t *&ptr) {
	T ret = *((T*)ptr);
	ptr += sizeof(T);
	return ret;
}
