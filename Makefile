CC=/usr/local/bin/afl-clang
CXX=/usr/local/bin/afl-clang++
CFLAGS=-Wall -std=c++14
OBJECTS=main.o
TARGET=dreadnot

$(TARGET): $(OBJECTS)
	$(CXX) $(CFLAGS) -o $(TARGET) $(OBJECTS)

$(OBJECTS): %.o : %.cpp
	$(CXX) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJECTS) $(TARGET)
