#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <stdlib.h> 
#include <iostream>
#include <string.h>

#define nCells 12
#define max_x 512
#define max_y 512
#define max_vect max_x

int8_t *** alloc_matrix();
void init_matrix(int8_t *** matrix, int8_t init);
void init_vecotr(int8_t * vector, int8_t init);
void free_matrix(int8_t *** matrix);
void handle_alloc_error(void *ptr);
int64_t * mvm(int8_t *** matrix, int8_t * vector, int cells);


#endif

