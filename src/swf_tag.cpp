#include "swf_tag.hpp"

swf_tag::swf_tag() {
	handle = nullptr;
	kind = 0;
	length = 0;
}

swf_tag::swf_tag(uint8_t **raw) {
	handle = *raw;
	uint16_t header = conv_copy<uint16_t>(handle);
	kind = header >> 6;
	length = header & 63;
	if (length == 0x3f) {
		length = conv_copy<uint32_t>(handle);
	}
	*raw += (handle - *raw) + length;
}
