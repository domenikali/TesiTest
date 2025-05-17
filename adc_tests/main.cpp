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

    uint64_t max_mask = ~(~1ULL << OUTPUT_SIZE+1);
    uint64_t min_mask = ~(~1ULL << OUTPUT_SIZE);

    std::cout<<std::bitset<64>(max_mask)<<std::endl;
    std::cout<<std::bitset<64>(min_mask)<<std::endl;


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