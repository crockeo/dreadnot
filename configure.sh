#!/usr/bin/env bash

command_exists () {
    type "$1" &> /dev/null ;
}

# git clone http://github.com/emeryberger/heap-layers.git

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

if [ $has_compiler = 0 ]
then
    echo "Could not find a compiler."
fi

if [ $has_compiler = 1 ]
then
    echo "CC=$c_compiler
CXX=$cpp_compiler
CFLAGS=-Wall -std=c11
CXFLAGS=-Wall -std=c++14
INCLUDES=-I./heap-layers
LINKS=
HEADERS=parse.hpp failure_list.hpp
OBJECTS=main.o parse.o failure_list.o grammar.o
TARGET=dreadnot

all: \$(TARGET) \$(POST)

\$(TARGET): \$(HEADERS) \$(OBJECTS)
	\$(CXX) \$(CXFLAGS) -o \$(TARGET) \$(OBJECTS)

\$(OBJECTS): %.o : %.cpp
	\$(CXX) \$(INCLUDES) \$(CXFLAGS) -c \$< -o \$@

clean:
	rm -rf Makefile \$(OBJECTS) \$(TARGET) \$(POST) \$(POST_OBJECTS)" > Makefile
fi
