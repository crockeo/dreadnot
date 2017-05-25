DEF_CC=clang
DEF_CXX=clang++
AFL_CC=/usr/local/bin/afl-clang
AFL_CXX=/usr/local/bin/afl-clang++
CFLAGS=-Wall -std=c++14
INCLUDES=-I./heap-layers
LINKS=
OBJECTS=main.o
TARGET=dreadnot

ifeq ($(AFL), 1)
	CC=$(AFL_CC)
	CXX=$(AFL_CXX)
else
	CC=$(DEF_CC)
	CXX=$(DEF_CXX)
endif

$(TARGET): $(OBJECTS)
	$(CXX) $(CFLAGS) -o $(TARGET) $(OBJECTS)

$(OBJECTS): %.o : %.cpp
	$(CXX) $(INCLUDES) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJECTS) $(TARGET)
