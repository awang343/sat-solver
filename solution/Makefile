CXX = g++
CXXFLAGS = -pedantic-errors -Wall -Wextra \
		   -Wconversion -Wsign-conversion \
		   -std=c++23

# Define directories
BIN_DIR = bin
BUILD_DIR = build
INC_DIR = include
SRC_DIR = src

SRCS = main.cpp timer.cpp dimacs_parser.cpp sat_instance.cpp dpll.cpp
OBJS = $(SRCS:.cpp=.o)
TARGET = main

# Adjust file paths
SRCS := $(addprefix $(SRC_DIR)/, $(notdir $(SRCS)))
OBJS := $(addprefix $(BUILD_DIR)/, $(notdir $(OBJS)))
TARGET = $(BIN_DIR)/main

all: $(TARGET)

$(TARGET): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

# Compile .cpp files into .o files inside bin/
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -I$(INC_DIR) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

.PHONY: all clean
