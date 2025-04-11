#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <stdlib.h> 
#include <iostream>
#include <string>
#include <cstring>

#define nCells 2
#define tile_size 128
#define tile_per_arry 4
#define n_sectors 4

#define max_x (tile_size*tile_per_arry)
#define max_y max_x
#define max_vect max_x


int8_t***** alloc_matrix();
void init_matrix(int8_t***** matrix, int8_t init);
void init_vecotr(int8_t * vector, int8_t init);
void free_matrix(int8_t***** matrix);
void handle_alloc_error(void *ptr);
int64_t * mvm(int8_t***** matrix, int8_t * vector,int sector);
std::string preatty_matrix(int8_t ***** matrix);
void create_matrix_conf_file(int8_t ***** matrix, int sector);
void create_vector_conf_file(int8_t *vector);
void create_result_conf_file(int64_t *result);



#endif

