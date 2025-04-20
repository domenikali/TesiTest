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
    srand(time(0));    
    for(int i=0;i<10;i++){
        adc((int64_t)rand(),true);
    }
}


void test(){
    
    int range = INT8_MAX - INT8_MIN + 1;

    int8_t temp = static_cast<int8_t>(INT8_MIN + (rand() % range));

    int8_t t1 = 0;
    int8_t t2 = 0;
    
    t1 = (temp >>4) & 0x0F;
    t2 = temp & 0x0F;

    int8_t result = ((t1<<4)+t2 );

    assert(result == temp);
}