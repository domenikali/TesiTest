#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <stdlib.h> 
#include <iostream>
#include <string>
#include <cstring>
#include <time.h>
#include <random>
#include <thread>
#include <chrono>
#include <cstdint>

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
void random_matrix(int8_t***** matrix);
void random_vector(int8_t * vector);
void free_matrix(int8_t***** matrix);
void handle_alloc_error(void *ptr);
std::string preatty_matrix(int8_t ***** matrix);
void create_matrix_conf_file(int8_t ***** matrix, int sector);
void create_vector_conf_file(int8_t *vector);
void create_result_conf_file(int64_t *result);


//mvm implementations:
/**@brief standard MVM impelemntation
 * multiply the matrix with the vector and return the result
 * @param matrix the matrix to be multiplied
 * @param vector the vector to be multiplied
 * @param sector the sector to be used
 * @return the result of the multiplication
*/
int64_t * mvm(int8_t***** matrix, int8_t * vector,int sector);

/**@breif multithreaded mvm implementation
 * each thread handles a column vector moltiplication, no concurrency protections
 * @param matrix the matrix to be multiplied
 * @param vector the vector to be multiplied
 * @param sector the sector to be used
 * @return the result of the multiplication
*/
int64_t * mvm_multithreaded(int8_t***** matrix, int8_t * vector, int sector);
void compute_colum(int8_t **colum,int8_t * vector, int64_t * result);


/**@brief multithreaded mvm second implementation
 * each thread handles a tile vector moltiplication, no concurrency protections
 * @param matrix the matrix to be multiplied
 * @param vector the vector to be multiplied
 * @param sector the sector to be used
 * @return the result of the multiplication
*/
int64_t * mvm_multithreaded_2(int8_t***** matrix, int8_t * vector, int sector);
void compute_tile(int8_t ***** matrix, int8_t * vector, int64_t * result, int sector, int tile);





#endif
