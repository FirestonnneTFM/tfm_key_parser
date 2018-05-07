CXX = g++
CXXFLAGS = -Wall -Wextra -Wpedantic -std=c++11 -Wshadow -O2
LDLIBS = -lz
TAG_SOURCES = $(wildcard ./src/swf_tag/*.cpp)
TAG_OBJECTS = $(TAG_SOURCES:.cpp=.o)
OBJECTS = $(TAG_OBJECTS) ./src/abc_executer.o ./src/abc_file.o	\
./src/abc_stream.o ./src/abc_version.o ./src/as3_op.o			\
./src/class_info.o ./src/cpool_info.o ./src/cwf_header.o		\
./src/exception_info.o ./src/int_stack.o ./src/method_info.o	\
./src/multiname_info.o ./src/script_info.o ./src/swf_file.o		\
./src/swf_tag.o ./src/traits_info.o
ENTRY_DUMPER = ./src/main_dumper.o
ENTRY_SNIFFER = ./src/main_sniffer.o

.PHONY: all debug clean dumper sniffer

all: dumper sniffer

debug: CXXFLAGS += -g
debug: all

dumper: $(OBJECTS) $(ENTRY_DUMPER)
	$(CXX) $(OBJECTS) $(ENTRY_DUMPER) $(CXXFLAGS) -o dumper $(LDLIBS)

sniffer: $(OBJECTS) $(ENTRY_SNIFFER)
	$(CXX) $(OBJECTS) $(ENTRY_SNIFFER) $(CXXFLAGS) -o sniffer $(LDLIBS)

clean:
	rm -f ./src/*.o ./src/swf_tag/*.o


