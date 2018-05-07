#include "abc_stream.hpp"

abc_stream::abc_stream() {
	data = nullptr;
	ptr = nullptr;
}

abc_stream::abc_stream(uint8_t *dat) {
	data = dat;
	ptr = dat;
}

void abc_stream::undo() {
	--ptr;
}

uint8_t abc_stream::next() {
	uint8_t value = *ptr;
	ptr += 1;
	return value;
}

uint8_t abc_stream::peek() {
	return *ptr;
}

int32_t abc_stream::s24() {
	int32_t value = ptr[0] | (ptr[1] << 8) | (ptr[2] << 16);
	ptr += 3;
	return value;
}

abc_stream &abc_stream::operator >>(uint8_t &value) {
	value = *ptr;
	ptr += 1;
	return *this;
}

abc_stream &abc_stream::operator >>(uint16_t &value) {
	value = *((uint16_t*)ptr);
	ptr += 2;
	return *this;
}

abc_stream &abc_stream::operator >>(uint32_t &value) {
	int i;
	value = 0;
	int pos = 0;
	int count = 0;
	bool next;
	do {
		i = *ptr;
		++ptr;
		next = (i >> 7) == 1;
		i &= 0x7f;
		value += (i << pos);
		++count;
		pos += 7;
	} while (next && count < 5);
	return *this;
}
	

abc_stream &abc_stream::operator >>(int8_t &value) {
	value = *((int8_t*)ptr);
	ptr += 1;
	return *this;
}

abc_stream &abc_stream::operator >>(int16_t &value) {
	value = *((int16_t*)ptr);
	ptr += 2;
	return *this;
}

abc_stream &abc_stream::operator >>(int32_t &value) {
	int i;
	value = 0;
	int pos = 0;
	int count = 0;
	bool next;
	do {
		i = *ptr;
		++ptr;
		next = (i >> 7) == 1;
		i &= 0x7f;
		value += (i << pos);
		count++;
		pos += 7;
		if (pos == 35) {
			if ((value >> 31) == 1) {
				value = -(value & 0x7fffffff);
			}
			break;
		}
	} while (next && count < 5);
	return *this;
}

abc_stream &abc_stream::operator >>(float &value) {
	value = *((float*)ptr);
	ptr += 4;
	return *this;
}

abc_stream &abc_stream::operator >>(double &value) {
	value = *((double*)ptr);
	ptr += 8;
	return *this;
}

abc_stream &abc_stream::operator >>(char *&value) {
	uint32_t len;
	*this >> len;
	value = new char[len + 1];
	unsigned i;
	for (i = 0; i < len; ++i, ++ptr) {
		value[i] = *ptr;
	}
	value[i] = '\0';
	return *this;
}
