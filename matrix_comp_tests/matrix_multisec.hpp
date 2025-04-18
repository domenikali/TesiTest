#ifndef MATRIX_MULTISEC_HPP
#define MATRIX_MULTISEC_HPP

#include "matrix.hpp"

int8_t * flat();
int64_t * flat_mvm(int8_t * matrix, int8_t * vector,int**sector);
void random_flat(int8_t * matrix);
void compute_flat_4(int8_t * matrix, int8_t * vector, int64_t * result, int i, int * sector);
int64_t *flat_4t(int8_t * matrix, int8_t * vector, int**sector);
void compute_flat_i(int8_t *matrix, int8_t * vector, int64_t * result, int *sector, int tile, int i,int inc);
int64_t * flat_8t(int8_t* matrix, int8_t * vector, int **sector);
int64_t * flat_16t(int8_t* matrix, int8_t * vector, int **sector);
int64_t * flat_32t(int8_t* matrix, int8_t * vector, int **sector);
void flat_matrix_config(int8_t * matrix, int **sector);


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