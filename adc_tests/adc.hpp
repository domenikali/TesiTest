#ifndef ADC_HPP
#define ADC_HPP

#include <string>
#include <chrono>
#include <cstdint>
#include <iostream>
#include <bitset>

#define OUTPUT_SIZE 11
#define CELL_SIZE 4


/**
 * @brief ADC simulator converter, cutting at OUTPUT_SIZE bits
 * This function converts a 64-bit signed integer (result of AIMC) to an array of unsigned 8-bit integers.
 * This function depend entierly on the size of the output defined by the JSON file, the array indexed zero contains the most significant byte.
 * @param input the input value to be converted
 * @return a pointer to the converted array
 */
uint8_t* adc(int64_t input,bool unsigned_conversion);
int64_t cell_weight_conversion();
uint64_t buffer_extraction(uint8_t *buffer,size_t size);


#endif