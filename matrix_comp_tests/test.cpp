#include <cstdint>

#include <string.h>
#include <stdlib.h> 
#include <time.h>
#include <iostream>
inline long long m_inedx(int s,int l,int y,int x){
    return (((s*8+l)*128+y)*512)+x;
}

void new_mvm(uint8_t*matrix,uint8_t*vector, int* layers,int*sectors,int64_t*result ){
    memset(result,0,512*8);

    int s=0;
    while(sectors[s]!=-1){
        
        for(int y=0;y<128;++y){
            for(int x=0;x<512;++x){
                int l=0;
                uint64_t wheight=0;
                while(layers[l]!=-1){
                    wheight+=matrix[m_inedx(sectors[s],layers[l],y,x)];
                    ++l;
                }
                result[sectors[s]*128+y]+=wheight*vector[x];
            }
        }
            
        s++;
    }

}


int main(){
    uint8_t* matrix = new uint8_t[8*512*512];
    uint8_t* vector = new uint8_t[512];
    int64_t* result = new int64_t[512*8];
    int* sectors = new int[4+1];
    int* layers = new int[8+1];
    srand(time(nullptr));

    for(int i=0;i<512;++i)vector[i]=static_cast<int8_t>(INT8_MIN + (std::rand()%(INT8_MAX - INT8_MIN + 1)));
    for(int i=0;i<8*512*512;++i)matrix[i]=static_cast<int8_t>(INT8_MIN + (std::rand()%(INT8_MAX - INT8_MIN + 1)));
    for(int i=0;i<4;++i)sectors[i]=i;
    sectors[4]=-1;
    layers[0]=0;
    layers[1]=1;
    layers[2]=-1;

    new_mvm(matrix,vector,layers,sectors,result);
    for(int i=0;i<512;++i){
        std::cout<<result[i]<<std::endl;
        
    }
    

    return 0;
}