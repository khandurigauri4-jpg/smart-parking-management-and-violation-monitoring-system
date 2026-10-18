CC = cc
CFLAGS = -Wall -Wextra -Iinclude -std=c11
CORE_SRC = src/queue.c src/stack.c src/linked_list.c
TEST_SRC = tests/test_main.c

all: test_runner dsa_bridge

test_runner: $(CORE_SRC) $(TEST_SRC)
	$(CC) $(CFLAGS) $^ -o $@

dsa_bridge: $(CORE_SRC) src/bridge.c
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f src/*.o test_runner test_runner.exe dsa_bridge

.PHONY: all clean
