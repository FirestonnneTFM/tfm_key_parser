#include "swf_file.hpp"
#include <fstream>
#include <zlib.h>
#include <string.h>

void swf_file::cwf_header::print() {
	std::cout << "sig: " << sig[0] << sig[1] << sig[2] << std::endl;
	std::cout << "version: " << +version << std::endl;
	std::cout << "size: " << file_size << std::endl; 
}

bool swf_file::cwf_header::is_sig_valid() {
	return sig[0] == 'C' && sig[1] == 'W' && sig[2] == 'S';
}

swf_file::swf_file(char *fname) {
	std::ifstream file;
	file.open(fname, std::ios::in | std::ios::binary | std::ios::ate);
	int fsize = file.tellg();
	file.close();
	
	file.open(fname, std::ios::in | std::ios::binary);
	if (! file.is_open())
		fatal("Could not open file");
	file.read((char*)&header, sizeof(cwf_header));
	if (! header.is_sig_valid()) {
		header.print();
		fatal("Header is not valid");
	}
	fsize -= sizeof(cwf_header);
	char *in_data = new char[fsize];
	file.read(in_data, fsize);
	file.close();
	handle = new uint8_t[header.file_size];
	
	z_stream stream;
	stream.zalloc = Z_NULL;
	stream.zfree = Z_NULL;
	stream.opaque = Z_NULL;
	stream.avail_in = (uInt)fsize;
	stream.next_in = (Bytef*)in_data;
	stream.avail_out = header.file_size;
	stream.next_out = (Bytef*)handle;

	inflateInit(&stream);
	inflate(&stream, Z_NO_FLUSH);
	inflateEnd(&stream);

	// read tags
	uint8_t *raw = handle;
	uint16_t bit_size = *raw >> 3;
	bit_size = bit_size * 4 + 5;
	for ( ; bit_size < 200; bit_size -= 8) {
		++raw;
	}
	// frame rate and frame count
	raw += 4;
	for ( ; ; ) {
		swf_tag tag(&raw);
		tags.push_back(tag);
		if (tag.kind == 0)
			break;
	}
	swf_tag abc_tag;
	get_tags(82, &abc_tag, 1);
	abc = abc_file::factory_from_bytes(abc_tag.get_handle());
	/*
	do {
		
		uint16_t header_0 = conv_copy<uint16_t>(raw);
		uint16_t tag_type = header_0 >> 6;
		uint32_t tag_len = header_0 & 63;
		if (tag_len == 0x3f) {
			tag_len = conv_copy<uint32_t>(swf);
		}
		if (tag_type == 87) {
			uint16_t tag = conv_copy<uint16_t>(swf);
			// skip ahead 4 bytes (reserved data)
			btags[i] = swf + 4;
			btag_lens[i] = tag_len - 6;
			// undo the read (length is not from this point)
			swf -= 2;
			++i;
			if (tag != i)
				throw "Tag and position mismatch";
		}
		if (tag_type == 76) {
			sym = swf;
		}
		swf += tag_len;
	} while (sym == nullptr || i < 12);
	*/

	delete [] in_data;
}

void swf_file::get_tags(uint16_t kind, swf_tag *out_tags, uint32_t n) {
	unsigned i = 0;
	for (auto item : tags) {
		if (item.kind == kind) {
			out_tags[i] = item;
			++i;
			if (i == n)
				return;
		}
	}
}

swf_file::~swf_file() {
	delete [] handle;
}
