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
#define cell_size 4
#define max_x (tile_size*tile_per_arry)
#define max_y max_x
#define max_vect max_x

typedef int8_t input_size_t;

int8_t***** alloc_matrix();
void init_matrix(int8_t***** matrix, int8_t init);
void init_vecotr(int8_t * vector, int8_t init);
void random_matrix(int8_t***** matrix);
void random_vector(input_size_t * vector);
void free_matrix(int8_t***** matrix);
void handle_alloc_error(void *ptr);
std::string preatty_matrix(int8_t ***** matrix);
void create_matrix_conf_file(int8_t ***** matrix, int sector);
void create_vector_conf_file(input_size_t *vector);
void create_result_conf_file(int64_t *result);

//mutlisectors
int8_t***** alloc_matrix_multisector();



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
int64_t * mvm_512_t(int8_t***** matrix, int8_t * vector, int sector);
void compute_colum(int8_t **colum,int8_t * vector, int64_t * result);


/**@brief multithreaded mvm 4 thread implementation
 * each thread handles a tile vector moltiplication, no concurrency protections
 * @param matrix the matrix to be multiplied
 * @param vector the vector to be multiplied
 * @param sector the sector to be used
 * @return the result of the multiplication
*/
int64_t * mvm_4_t(int8_t***** matrix, int8_t * vector, int sector);
void compute_tile(int8_t ***** matrix, int8_t * vector, int64_t * result, int sector, int tile);


/**@breif multithreaded mvm 8 thread implementation
 * each thread handles a half a tile
 * @param matrix the matrix to be multiplied
 * @param vector the vector to be multiplied
 * @param sector the sector to be used
 * @return the result of the multiplication
 */
int64_t * mvm_8_t(int8_t***** matrix, int8_t * vector, int sector);
void compute_tile_max_t(int8_t ***** matrix, int8_t * vector, int64_t * result, int sector, int tile, int i,int inc);

/**@breif multithreaded mvm 16 thread implementation
 * each thread handles a quarter of a tile
 * @param matrix the matrix to be multiplied
 * @param vector the vector to be multiplied
 * @param sector the sector to be used
 * @return the result of the multiplication
 */
int64_t * mvm_16_t(int8_t***** matrix, int8_t * vector, int sector);

/**@breif multithreaded mvm 20 thread implementation
 * each thread handles a fift of a tile
 * @param matrix the matrix to be multiplied
 * @param vector the vector to be multiplied
 * @param sector the sector to be used
 * @return the result of the multiplication
 */
int64_t * mvm_20_t(int8_t***** matrix, int8_t * vector, int sector);
int64_t * mvm_32_t(int8_t***** matrix, int8_t * vector, int sector);







#endif
