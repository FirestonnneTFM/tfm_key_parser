#pragma once

#include "common.hpp"

class abc_stream {
private:
	const uint8_t *data;
protected:
	uint8_t *ptr;
public:
	abc_stream();
	abc_stream(uint8_t *);
	void undo();
	uint8_t next();
	uint8_t peek();
	int32_t s24();
	abc_stream &operator >>(uint8_t &);
	abc_stream &operator >>(uint16_t &);
	abc_stream &operator >>(uint32_t &);
	abc_stream &operator >>(int8_t &);
	abc_stream &operator >>(int16_t &);
	abc_stream &operator >>(int32_t &);
	abc_stream &operator >>(float &);
	abc_stream &operator >>(double &);
	abc_stream &operator >>(char *&);
};
