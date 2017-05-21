CC=/usr/local/bin/afl-clang++
CFLAGS=-Wall -std=c++14
OBJECTS=main.o
TARGET=dreadnot

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS)

$(OBJECTS): %.o : %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJECTS) $(TARGET)
