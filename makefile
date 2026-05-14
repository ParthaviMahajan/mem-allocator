CC = gcc

CFLAGS = -Wall -Wextra -g

TARGET = allocator

SRC = src/allocator.c tests/test_allocator.c

all:
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

run:
	./$(TARGET)

clean:
	rm -f $(TARGET)