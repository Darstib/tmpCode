# Makefile

CXX = g++
CXXFLAGS = -std=c++17
INCLUDES = -I./include

SRC_DIR = src
BIN_DIR = bin

VECTOR_SRC = $(SRC_DIR)/vectorTest.cpp
CONTAINER_SRC = $(SRC_DIR)/containerTest.cpp
DATATYPE_SRC = $(SRC_DIR)/dataTypeTest.cpp

VECTOR_BIN = $(BIN_DIR)/vectorTest
CONTAINER_BIN = $(BIN_DIR)/containerTest
DATATYPE_BIN = $(BIN_DIR)/dataTypeTest

.PHONY: all vector container datatype clean

all: container datatype vector

vector: $(VECTOR_SRC)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $< -o $(VECTOR_BIN) && ./$(VECTOR_BIN) 2>/dev/null

container: $(CONTAINER_SRC)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $< -o $(CONTAINER_BIN) && ./$(CONTAINER_BIN) 2>/dev/null

datatype: $(DATATYPE_SRC)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $< -o $(DATATYPE_BIN) && ./$(DATATYPE_BIN) 2>/dev/null

clean:
	rm -rf $(BIN_DIR)