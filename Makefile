# Compiler
CC = gcc

# Source and output directories
SRC = src/main.c
OUT_DIR = bin
TARGET = $(OUT_DIR)/main

# Build and run
all: $(OUT_DIR) $(TARGET)
	./$(TARGET)

# Ensure the output directory exists
$(OUT_DIR):
	mkdir -p $(OUT_DIR)

# Compile the source file
$(TARGET): $(SRC)
	$(CC) $(SRC) -o $(TARGET)

# Clean build files
clean:
	rm -rf $(OUT_DIR)
