#pragma once

#include "common.hpp"

class swf_tag_DefineBinaryData {
public:
	uint16_t ch_id;
	uint8_t *binary_data;
	uint32_t length;
	swf_tag_DefineBinaryData();
	swf_tag_DefineBinaryData(swf_tag *tag);
};
