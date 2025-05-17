#ifndef MATRIX_MULTISEC_HPP
#define MATRIX_MULTISEC_HPP

#include "matrix.hpp"
#include <bitset>

typedef uint8_t pcm_size_t;


inline uint64_t index(int sect,int i, int j, int k, int x);

pcm_size_t * flat(uint64_t *size);
int64_t * flat_mvm(int8_t * matrix, input_size_t * vector,int**sector);
void flat_mvm_weight(pcm_size_t * matrix, input_size_t * vector,int**sector,int64_t * result);
void init_mvm(int**sectors);
void random_flat(pcm_size_t * matrix);
void compute_flat_2(pcm_size_t * matrix, input_size_t * vector, int64_t * result, int m, int ** sector);
int64_t *flat_4t(pcm_size_t * matrix, input_size_t * vector, int**sector, int64_t * result);
void compute_flat_i(pcm_size_t *matrix, input_size_t * vector, int64_t * result, int *sector, int tile, int i,int inc);
int64_t * flat_2t(pcm_size_t* matrix, input_size_t * vector, int **sector,int64_t * result);
int64_t * flat_8t(pcm_size_t* matrix, input_size_t * vector, int **sector,int64_t * result);
int64_t * flat_16t(pcm_size_t* matrix, input_size_t * vector, int **sector,int64_t * result);
int64_t * flat_32t(pcm_size_t* matrix, input_size_t * vector, int **sector,int64_t * result);
int64_t * flat_64t(pcm_size_t* matrix, input_size_t * vector, int **sector,int64_t * result);
int64_t * flat_128t(pcm_size_t* matrix, input_size_t * vector, int **sector,int64_t * result);
int64_t * flat_512t(pcm_size_t* matrix, input_size_t * vector, int **sector,int64_t * result);

void load(int64_t index, int64_t value, pcm_size_t * matrix);


void flat_matrix_config(pcm_size_t * matrix, int **sector);


int8_t***** alloc_matrix_multisector();
void create_matrix_conf_file_multisec(int8_t ***** matrix, int *sector);
void free_matrix_multisector(int8_t***** matrix);
void random_matrix_multisec(int8_t***** matrix);
int64_t * mvm_multisect_performance(int8_t***** matrix, int8_t * vector,int **sector);

void create_matrix_conf_file_2(int8_t ***** matrix, int *secotr);
int64_t * mvm_multisect(int8_t***** matrix, int8_t * vector,int **sector);

void compute_tile_4_std(int8_t ***** matrix, int8_t * vector, int64_t * result, int *sector, int tile);
int64_t * mvm_std_4_t(int8_t***** matrix, int8_t * vector, int **sector);

void compute_tile_4_fast(int8_t ***** matrix, int8_t * vector, int64_t * result, int *sector, int tile);
int64_t * mvm_fast_4_t(int8_t***** matrix, int8_t * vector, int **sector);
#endif