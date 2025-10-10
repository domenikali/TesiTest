#ifndef BTTD_H
#define BTTD_H
//Back To The Drawingboard
#include "matrix_multisec.hpp"
#include <atomic> 
extern unsigned int computations; 

#define PREFETCH_DISTANCE 32

inline long long m_inedx(int s,int l,int y,int x){
    return (((s*8+l)*128+y)*512)+x;
}

inline long long m_inedx_2(int s,int l,int y,int x){
    return (((s*128+y)*512+x)*8)+l;
}

/**
 * |L0(0),L1(0),L2(0),.......
 */

void new_mvm(pcm_size_t*matrix,input_size_t*vector, int** layers,int*sectors,int64_t*resutl );
void print_mvm(pcm_size_t*matrix, int** layers,int*sectors);
void acc_test(pcm_size_t*matrix, int* layers,int*sectors);

void new_mvm_2(pcm_size_t*matrix,input_size_t*vector, int**layers,int*sectors,int64_t*resutl );
void new_mvm_3(pcm_size_t* matrix, input_size_t* vector,  int**  layers, int* sectors, int64_t* result);
void new_mvm_4(pcm_size_t* matrix, input_size_t* vector,  int**  layers, int* sectors, int64_t* result);



void multi_thread_sector(int s, pcm_size_t* matrix, input_size_t* vector,  int**  layers, int64_t* result);
void new_mvm_mtd_4(pcm_size_t* matrix, input_size_t* vector,  int**  layers, int* sectors, int64_t* result);


void new_mvm_mtd_8(pcm_size_t* matrix, input_size_t* vector,  int** layers, int* sectors, int64_t* result);
void multi_thread_layer( pcm_size_t* matrix, int s,input_size_t* vector, std::atomic<int64_t>* result);


void new_mvm_mtd_16(pcm_size_t* matrix, input_size_t* vector,  int** layers, int* sectors, int64_t* result);
void multi_thread_layer_2( pcm_size_t* matrix, int s,input_size_t* vector, std::atomic<int64_t>* result,int y_start,int end);



#endif 