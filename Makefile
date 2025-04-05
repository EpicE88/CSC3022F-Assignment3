# Makefile based on a generic template for compiling C++ programs.
# Author: Ethan Ngwetjana
# Date: 05/04/2025

# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -Wall -Wextra -std=c++17 -O2

# Directories
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

# Source files
SRCS = $(filter-out $(SRC_DIR)/unittest.cpp, $(wildcard $(SRC_DIR)/*.cpp))

# Test source file
TEST_SRC = $(SRC_DIR)/UnitTests.cpp

# Object files
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))

# Test object file
TEST_OBJ = $(OBJ_DIR)/UnitTests.o

# Executable name
TARGET = $(BIN_DIR)/findcomp

# Test executable name
TEST_TARGET = $(BIN_DIR)/unittest

# Default target
all: $(TARGET)

# Link the object files to create the executable
$(TARGET): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compile source files into object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Build and run unit tests
test: $(TEST_TARGET)
	@./$(TEST_TARGET)

# Link test object files to create the test executable
$(TEST_TARGET): $(filter-out $(OBJ_DIR)/main.o, $(OBJS)) $(TEST_OBJ)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Clean up build files
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

# Phony targets
.PHONY: all clean