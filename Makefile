# Specify CPP20 standard
CXXFLAGS = -std=c++20
CXX = g++

SRC_DIR = src
INCLUDE_DIR = include
OBJ_DIR = obj
BIN_DIR = bin
EXE = $(BIN_DIR)/main

# Get all the source files in src directory
SOURCE_FILES = $(wildcard $(SRC_DIR)/*.cpp)
# Get all the header files in include directory
OBJ_FILES = $(SOURCE_FILES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

.PHONY: all clean printer

all: $(EXE)

$(BIN_DIR) $(OBJ_DIR):
	mkdir -p $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@ -I$(INCLUDE_DIR)

$(EXE): $(OBJ_FILES) | $(BIN_DIR)
	$(CXX) $^ -o $@

clean:
	rm -rf $(BIN_DIR) $(OBJ_DIR)

