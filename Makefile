DEF_CC=clang
DEF_CXX=clang++
AFL_CC=/usr/local/bin/afl-clang
AFL_CXX=/usr/local/bin/afl-clang++
CFLAGS=-Wall -std=c++14
INCLUDES=-I./heap-layers
LINKS=
HEADERS=parse.hpp heap.hpp
OBJECTS=main.o parse.o
TARGET=dreadnot

ifeq ($(AFL), 1)
	CC=$(AFL_CC)
	CXX=$(AFL_CXX)
else
	CC=$(DEF_CC)
	CXX=$(DEF_CXX)
endif

$(TARGET): $(HEADERS) $(OBJECTS)
	$(CXX) $(CFLAGS) -o $(TARGET) $(OBJECTS)

$(OBJECTS): %.o : %.cpp
	$(CXX) $(INCLUDES) $(CFLAGS) -c $< -o $@

test: $(TARGET)
	afl-fuzz -i fuzz/case/ -o fuzz/find/ -x fuzz/keywords.txt -- ./$(TARGET)

clean:
	rm -rf $(OBJECTS) $(TARGET)
