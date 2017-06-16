#!/usr/bin/env bash

command_exists () {
    type "$1" &> /dev/null ;
}

git clone http://github.com/emeryberger/heap-layers.git

# Checking for GCC or clang.
has_compiler=0
if command_exists gcc && command_exists g++
then
    c_compiler="gcc"
    cpp_compiler="g++"
    has_compiler=1
elif command_exists clang && command_exists clang++
then
    c_compiler="clang"
    cpp_compiler="clang++"
    has_compiler=1
fi

# Checking for the AFL compiler.
if command_exists afl-clang && command_exists afl-clang++
then
    has_compiler=1
else
    has_compiler=0
fi

if [ $has_compiler = 0 ]
then
    echo "Could not find a required compiler. afl-clang and either GCC or clang."
fi

if [ $has_compiler = 1 ]
then
    echo "DEF_CC=$c_compiler
DEF_CXX=$cpp_compiler
AFL_CC=afl-clang
AFL_CXX=afl-clang++
CFLAGS=-Wall -std=c11
CXFLAGS=-Wall -std=c++14
INCLUDES=-I./heap-layers
LINKS=
HEADERS=parse.hpp heap.hpp
OBJECTS=main.o parse.o
TARGET=dreadnot

POST_OBJECTS=postprocess.o
POST=postprocess.so

ifeq (\$(AFL), 1)
	CC=\$(AFL_CC)
	CXX=\$(AFL_CXX)
else
	CC=\$(DEF_CC)
	CXX=\$(DEF_CXX)
endif

all: \$(TARGET) \$(POST)

\$(TARGET): \$(HEADERS) \$(OBJECTS)
	\$(CXX) \$(CXFLAGS) -o \$(TARGET) \$(OBJECTS)

\$(OBJECTS): %.o : %.cpp
	\$(CXX) \$(INCLUDES) \$(CXFLAGS) -c \$< -o \$@

\$(POST): \$(POST_OBJECTS)
	\$(CC) -shared \$(CFLAGS) -o \$(POST) \$(POST_OBJECTS)

\$(POST_OBJECTS): %.o : %.c
	\$(CC) -fPIC \$(INCLUDES) \$(CFLAGS) -c \$< -o \$@

test: \$(TARGET) \$(POST)
	AFL_POST_LIBRARY=./postprocess.so afl-fuzz -i fuzz/case/ -o fuzz/find/ -x fuzz/keywords.txt -- ./\$(TARGET)

clean:
	rm -rf \$(OBJECTS) \$(TARGET) \$(POST) \$(POST_OBJECTS)" > Makefile
fi
