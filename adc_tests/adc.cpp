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

int64_t cell_weight_conversion(){
        
    int range = INT8_MAX - INT8_MIN + 1;
    uint8_t mask = 0xFF;
    mask>>=8-CELL_SIZE;
    uint64_t negative = 1 << ((CELL_SIZE*4)-1);
    
    int8_t * arr = new int8_t[4];
    for(int i=0;i<4;i++){
        arr[i] = (static_cast<int8_t>(INT8_MIN + (rand() % range))& mask);
        std::cout<<std::bitset<8>(arr[i])<<std::endl;
    }
    int64_t res=static_cast<int64_t>(0);
    std::cout<<std::bitset<64>(res)<<std::endl;


    
    for(int i=0;i<4;i++){
        res = (res<<CELL_SIZE) | (arr[i]);
    }
    if((res&negative)!=0){
        uint64_t m =(~0ULL << (CELL_SIZE*4));
        res = static_cast<int64_t>(static_cast<uint64_t>(res) | m);

    }

    std::cout<<std::bitset<64>(res)<<std::endl;
    std::cout<<res<<std::endl;
    return res;
}