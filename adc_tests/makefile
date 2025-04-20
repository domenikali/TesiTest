
# Compiler and flags
CC = g++
CCFLAGS = -Wall -Wextra -std=c++20 -pthread -g

# Files
# Assume main.cpp and matrix.cpp are in the current directory.
MAIN = main.cpp
ADC_SRC = adc.cpp
LOGGER_SRC = ../logger.cpp

OBJS = main.o adc.o logger.o

TARGET = adc_tests

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CCFLAGS) -o $(TARGET) $(OBJS)

main.o: main.cpp adc.hpp ../logger.hpp
	$(CC) $(CCFLAGS) -c main.cpp -o main.o

adc.o: adc.cpp adc.hpp 
	$(CC) $(CCFLAGS) -c adc.cpp -o adc.o

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
	
	@echo "Run tests"
	./$(TARGET)

.PHONY: all clean
