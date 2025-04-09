
# Compiler and flags
CC = g++
CCFLAGS = -Wall -Wextra -std=c++11

# Files
# Assume main.cpp and matrix.cpp are in the current directory.
MAIN = main.cpp
MATRIX_SRC = matrix.cpp
LOGGER_SRC = ../logger.cpp

OBJS = main.o matrix.o logger.o

TARGET = matrix_tests

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CCFLAGS) -o $(TARGET) $(OBJS)

main.o: main.cpp matrix.hpp ../logger.hpp
	$(CC) $(CCFLAGS) -c main.cpp -o main.o

matrix.o: matrix.cpp matrix.hpp 
	$(CC) $(CCFLAGS) -c matrix.cpp -o matrix.o

logger.o: ../logger.cpp ../logger.hpp
	$(CC) $(CCFLAGS) -c ../logger.cpp -o logger.o

clean:
	rm -f $(OBJS) $(TARGET)

clean_logs:
	rm -f *.txt

.PHONY: all clean
