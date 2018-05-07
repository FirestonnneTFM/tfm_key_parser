#!/bin/bash

wget http://transformice.com/Transformice.swf
./dumper Transformice.swf tfm.swf
unlink Transformice.swf
./sniffer tfm.swf out.bin
unlink tfm.swf

