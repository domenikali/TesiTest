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
    srand(time(NULL));    
    // for(int i=0;i<10;i++){
    //     adc((int64_t)rand(),true);
    // }
    //cell_weight_conversion();

    uint8_t n = (1 << CELL_SIZE) - 1;
    std::cout<<std::bitset<8>(n)<<std::endl;

    
}


void test(){

    
}