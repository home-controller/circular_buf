# Tool definitions
CC ?= gcc
CXX ?= g++

# Settings
SRC_DIR = ./src
TEST_DIR = ./tests
BUILD_DIR = ./build
NAME = app.elf

# Search path for header files
CFLAGS += -I$(SRC_DIR)/average

# List module source files
ifeq ($(wildcard $(TEST_DIR)/main.c),)
  CPP_SOURCES = $(TEST_DIR)/main.cpp
else
  CPP_SOURCES = $(TEST_DIR)/main.c
endif

CPP_SOURCES += $(wildcard $(SRC_DIR)/*.cpp)

# Compiler flags
CXXFLAGS += -Wall -std=c++11

# Linker flags
LDFLAGS += 

# Generate names for output object files (*.o)
CPP_OBJECTS = $(patsubst %.cpp, $(BUILD_DIR)/%.o, $(CPP_SOURCES))

# Default rule: build application
.PHONY: all
all: $(NAME)

# Build components
$(BUILD_DIR)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Build the target application
.PHONY: $(NAME)
$(NAME): $(CPP_OBJECTS)
	$(CXX) $(CPP_OBJECTS) -o $(BUILD_DIR)/$(NAME) $(LDFLAGS)

# Remove compiled object files
.PHONY: clean
clean:
	rm -f $(CPP_OBJECTS)

# Run tests
.PHONY: test
test:
	make -C $(TEST_DIR)
	
# Clean tests
.PHONY: test_clean
test_clean:
	make -C $(TEST_DIR) clean
