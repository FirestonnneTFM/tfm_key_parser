#pragma once

#include "common.hpp"

class swf_tag_SymbolClass {
public:
	uint16_t count;
	uint16_t *ch_ids;
	char **names;
	swf_tag_SymbolClass(swf_tag *tag);
	~swf_tag_SymbolClass();
};
