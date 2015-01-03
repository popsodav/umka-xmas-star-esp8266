#!/bin/sh

cd mman-win32
make clean && make
cd ..
make -f Makefile.windows clean && make -f Makefile.windows
