CXX = g++
CXXFLAGS = -std=c++11 -O2 -Wall
INCLUDE_DIR = include
OBJ_DIR = bin
SRC_DIR = src

_OBJS = boyer_moore.o brute_force.o kmp.o main.o sellers.o ukkonen.o utils.o
OBJS = $(patsubst %, $(OBJ_DIR)/%, $(_OBJS))

pmt: $(OBJS)
	$(CXX) -I $(INCLUDE_DIR) $(OBJS) -o $(OBJ_DIR)/pmt $(CXXFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	mkdir -p $(OBJ_DIR)
	$(CXX) -I $(INCLUDE_DIR) -c -o $@ $< $(CXXFLAGS)

.PHONY: clean

clean:
	rm -rf $(OBJ_DIR)

