DEF_CC=clang
DEF_CXX=clang++
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

ifeq ($(AFL), 1)
	CC=$(AFL_CC)
	CXX=$(AFL_CXX)
else
	CC=$(DEF_CC)
	CXX=$(DEF_CXX)
endif

all: $(TARGET) $(POST)

$(TARGET): $(HEADERS) $(OBJECTS)
	$(CXX) $(CXFLAGS) -o $(TARGET) $(OBJECTS)

$(OBJECTS): %.o : %.cpp
	$(CXX) $(INCLUDES) $(CXFLAGS) -c $< -o $@

$(POST): $(POST_OBJECTS)
	$(CC) -shared $(CFLAGS) -o $(POST) $(POST_OBJECTS)

$(POST_OBJECTS): %.o : %.c
	$(CC) $(INCLUDES) $(CFLAGS) -c $< -o $@

test: $(TARGET) $(POST)
	AFL_POST_LIBRARY=./postprocess.so afl-fuzz -i fuzz/case/ -o fuzz/find/ -x fuzz/keywords.txt -- ./$(TARGET)

clean:
	rm -rf $(OBJECTS) $(TARGET) $(POST) $(POST_OBJECTS)
