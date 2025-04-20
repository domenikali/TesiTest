#include "adc.hpp"

uint8_t * adc(int64_t input,bool unsigned_conversion) {
    //size parameter
    size_t size = OUTPUT_SIZE%(sizeof(uint8_t))+1;
    size+=((OUTPUT_SIZE-size)/(sizeof(uint8_t)*8));
    //output buffer allocation
    uint8_t *output = new uint8_t[size];
    if(output == nullptr) {
        std::cerr << "Memory allocation failed" << std::endl;
        return nullptr;
    }
    //output buffer initialization
    for (size_t i = 0; i < size; ++i) {
        output[size-i-1] = input >> (i * 8) & 0xFF;
    }  

    //clipping
    uint8_t mask=0xFF;
    int u_conversion = unsigned_conversion ? 1 : 0; 
    mask>>=(8-(OUTPUT_SIZE)+u_conversion)%8;

    //masking most significant byte
    output[0] &= mask;
    std::cout<<std::bitset<64>(input)<<std::endl;
    for (size_t i = 0; i < size; ++i) {
        std::cout<<std::bitset<8>(output[i])<<std::endl;
    }

    return output;
}