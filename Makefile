CC = gcc
CFLAGS = -Wall -g -I./src -I./src/common
SRC_DIR = ./src
OBJ_DIR = ./build/obj
BIN_DIR = ./build/bin
TEST_DIR = ./tests

SRCS = $(shell find $(SRC_DIR) -name '*.c' -or -name '*.cpp')
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
TARGET = $(BIN_DIR)/a++c

# Build the language
all: $(TARGET)

$(TARGET): $(OBJS)
	@mkdir -p $(BIN_DIR)  # This line should start with a tab
	$(CC) $(CFLAGS) -o $@ $^  # This line should also start with a tab


$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Debug build
debug: CFLAGS += -g -DDEBUG
debug: clean all

# Release build
release: CFLAGS += -O3
release: clean all

# Run tests
test: all
	@$(MAKE) -C $(TEST_DIR)

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

.PHONY: all clean debug release test
