
# Compiler and flags
CC = g++
CCFLAGS = -Wall -Wextra -std=c++20 -pthread -g

# Files
# Assume main.cpp and matrix.cpp are in the current directory.
MAIN = main.cpp
MATRIX_SRC = matrix.cpp
LOGGER_SRC = ../logger.cpp
MATRIX_M_SRC = matrix_multisec.cpp

OBJS = main.o matrix.o matrix_multisec.o logger.o

TARGET = matrix_tests

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CCFLAGS) -o $(TARGET) $(OBJS)

main.o: main.cpp matrix.hpp ../logger.hpp
	$(CC) $(CCFLAGS) -c main.cpp -o main.o

matrix.o: matrix.cpp matrix.hpp 
	$(CC) $(CCFLAGS) -c matrix.cpp -o matrix.o
	
matrix_multisec.o: matrix_multisec.cpp matrix.hpp ../logger.hpp
	$(CC) $(CCFLAGS) -c matrix_multisec.cpp -o matrix_multisec.o

logger.o: ../logger.cpp ../logger.hpp
	$(CC) $(CCFLAGS) -c ../logger.cpp -o logger.o

clean:
	rm -f $(OBJS) $(TARGET)

clean_logs:
	rm -f *.txt
	rm -fr massif.out.*
	rm -rf ./result/*.conf

make checks:
	python just_to_check.py

make run:
	@echo "Delate old conf files"
	rm -rf ./result/*.conf
	mkdir -p ./result
	@echo "Run tests"
	./$(TARGET)

.PHONY: all clean
