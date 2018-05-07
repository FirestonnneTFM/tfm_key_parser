#pragma once

#include "common.hpp"
#include "abc_file.hpp"
#include "swf_tag.hpp"
#include <vector>

class swf_file {
private:
	class cwf_header {
	public:
		char sig[3];
		uint8_t version;
		uint32_t file_size;
		void print();
		bool is_sig_valid();
	};
	
	uint8_t *handle;
	cwf_header header;
	abc_file *abc;
	std::vector<swf_tag> tags;
public:
	uint8_t *get_handle() {
		return handle;
	}
	uint8_t get_version() {
		return header.version;
	}
	uint32_t get_file_size() {
		return header.file_size;
	}
	abc_file *get_abc_file() {
		return abc;
	}
	void get_tags(uint16_t, swf_tag *, uint32_t);

	swf_file(char *);
	~swf_file();
};


