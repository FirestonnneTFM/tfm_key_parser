#include "DefineBinaryData.hpp"

swf_tag_DefineBinaryData::swf_tag_DefineBinaryData() {
	ch_id = 0;
	binary_data = nullptr;
	length = 0;
}

swf_tag_DefineBinaryData::swf_tag_DefineBinaryData(swf_tag *tag) {
	binary_data = tag->get_handle();
	ch_id = conv_copy<uint16_t>(binary_data);
	// reserved uint32_t
	binary_data += 4;
	length = tag->length - 6;
}
