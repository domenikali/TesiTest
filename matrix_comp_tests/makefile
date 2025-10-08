# Compiler and flags
CC = g++
CXXFLAGS = -Wall -Wextra -std=c++20 -pthread -g 

# Directories
SRC_DIR = .
OBJ_DIR = .
PARENT_DIR = ..

# Source files
SRCS = $(SRC_DIR)/main.cpp \
       $(SRC_DIR)/matrix.cpp \
       $(SRC_DIR)/matrix_multisec.cpp \
       $(SRC_DIR)/bttd.cpp \
       $(PARENT_DIR)/logger.cpp

# Object files
OBJS = $(OBJ_DIR)/main.o \
       $(OBJ_DIR)/matrix.o \
       $(OBJ_DIR)/matrix_multisec.o \
       $(OBJ_DIR)/bttd.o \
       $(OBJ_DIR)/logger.o

# Target executable
TARGET = matrix_tests

# Phony targets
.PHONY: all clean clean_logs run checks

# Default target
all: $(TARGET)

# Link object files to create the target executable
$(TARGET): $(OBJS)
	$(CC) $(CXXFLAGS) -o $(TARGET) $(OBJS)

# Compilation rules
$(OBJ_DIR)/main.o: $(SRC_DIR)/main.cpp $(SRC_DIR)/matrix.hpp $(PARENT_DIR)/logger.hpp
	$(CC) $(CXXFLAGS) -c $(SRC_DIR)/main.cpp -o $@

$(OBJ_DIR)/matrix.o: $(SRC_DIR)/matrix.cpp $(SRC_DIR)/matrix.hpp
	$(CC) $(CXXFLAGS) -c $(SRC_DIR)/matrix.cpp -o $@

$(OBJ_DIR)/matrix_multisec.o: $(SRC_DIR)/matrix_multisec.cpp $(SRC_DIR)/matrix.hpp $(PARENT_DIR)/logger.hpp
	$(CC) $(CXXFLAGS) -c $(SRC_DIR)/matrix_multisec.cpp -o $@

$(OBJ_DIR)/logger.o: $(PARENT_DIR)/logger.cpp $(PARENT_DIR)/logger.hpp
	$(CC) $(CXXFLAGS) -c $(PARENT_DIR)/logger.cpp -o $@

$(OBJ_DIR)/bttd.o: $(SRC_DIR)/bttd.cpp $(SRC_DIR)/matrix.hpp $(SRC_DIR)/bttd.hpp
	$(CC) $(CXXFLAGS) -c $(SRC_DIR)/bttd.cpp -o $@

# Run the executable
run: $(TARGET)
	@echo "Deleting old conf files..."
	rm -f ./result/*.conf
	mkdir -p ./result
	@echo "Running tests..."
	./$(TARGET)

# Run checks
checks:
	python just_to_check.py

# Clean up object files and the executable
clean:
	rm -f $(OBJS) $(TARGET)

# Clean up log and output files
clean_logs:
	rm -f *.txt
	rm -rf massif.out.*
	rm -rf ./result/*.conf
