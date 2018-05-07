#include "SymbolClass.hpp"
#include <string.h>

swf_tag_SymbolClass::swf_tag_SymbolClass(swf_tag *tag) {
	uint8_t *handle = tag->get_handle();
	count = conv_copy<uint16_t>(handle);
	ch_ids = new uint16_t[count];
	names = new char*[count];
	for (int i = 0; i < count; ++i) {
		ch_ids[i] = conv_copy<uint16_t>(handle);
		names[i] = (char*)handle;
		handle += strlen(names[i]) + 1;
	}
}

swf_tag_SymbolClass::~swf_tag_SymbolClass() {
	delete [] ch_ids;
	delete [] names;
}
