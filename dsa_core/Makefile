CC = gcc
CFLAGS = -Wall -Wextra -Iinclude -std=c11
SRC = src/queue.c src/stack.c src/linked_list.c
OBJ = $(SRC:.c=.o)
TEST_SRC = tests/test_main.c
TARGET = test_runner

all: $(TARGET)

$(TARGET): $(OBJ) $(TEST_SRC)
	$(CC) $(CFLAGS) $^ -o $@

src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f src/*.o $(TARGET) $(TARGET).exe

.PHONY: all clean