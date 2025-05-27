#include "adc.hpp"
#include <iostream>
#include <thread>
#include <chrono>
#include <cstdint>
#include <string>
#include "../logger.hpp"
#include <time.h>
#include <bitset>
#include <random>
#include <cstdlib>
#include <cassert>


void test();

Logger logger("adc.log");
int main(){

    size_t size =4;
    uint8_t *buff = new uint8_t[size];

    for(int i=0;i<size;i++){

        buff[i] = static_cast<uint8_t>(0b10101010);
        std::cout<<std::bitset<8>(buff[i])<<std::endl;

    }
    uint64_t value = buffer_extraction(buff,size);
    std::cout<<std::bitset<64>(value)<<std::endl;


    // srand(time(NULL));    
    // for(int i=0;i<10;i++){
    //     adc((int64_t)rand(),true);
    // }
    //cell_weight_conversion();

    // uint8_t n = (1 << CELL_SIZE) - 1;
    // std::cout<<std::bitset<8>(n)<<std::endl;

    
}


void test(){

    
}