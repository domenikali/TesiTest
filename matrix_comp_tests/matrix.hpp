#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <stdlib.h> 
#include <iostream>
#include <string.h>

#define nCells 2
#define tile_size 4
#define tile_per_arry 2
#define n_sectors 2

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


#endif

