#pragma once

#include "common.hpp"

class swf_tag {
private:
	uint8_t *handle;
public:
	uint16_t kind;
	uint32_t length;
	uint8_t *get_handle() {
		return handle;
	}
	swf_tag();
	swf_tag(uint8_t **raw);
};
