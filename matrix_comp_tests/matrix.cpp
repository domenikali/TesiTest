#include "matrix.hpp"

int8_t *** alloc_matrix(){
  int8_t *** matrix = new int8_t**[nCells];
  handle_alloc_error(matrix);
  for(int i=0;i<nCells;i++){
    matrix[i] = new int8_t*[max_x];
    handle_alloc_error(matrix[i]);
    for(int j=0;j<max_x;j++){
      matrix[i][j] = new int8_t[max_y];
      handle_alloc_error(matrix[i][j]);
    }
  }
  return matrix;
}

void init_matrix(int8_t *** matrix, int8_t init){
  for(int i=0;i<nCells;i++){
    for(int j=0;j<max_x;j++){
      for(int k=0;k<max_y;k++){
        matrix[i][j][k] = init;
      }
    }
  }
}

void init_vecotr(int8_t * vector, int8_t init){
  for(int i=0;i<max_vect;i++){
    vector[i] = init;
  }
}

void handle_alloc_error(void *ptr){
  if(ptr == NULL){
    std::cerr << "Memory allocation failed" << std::endl;
    exit(EXIT_FAILURE);
  }
}

void free_matrix(int8_t *** matrix){
  for(int i=0;i<nCells;i++){
    for(int j=0;j<max_x;j++){
      delete[] matrix[i][j];
    }
    delete[] matrix[i];
  }
  delete[] matrix;
} 

int64_t * mvm(int8_t *** matrix, int8_t * vector, int cells){
  int64_t * result = new int64_t[max_vect];
  handle_alloc_error(result);
  for(int i=0;i<max_x;i++){
    result[i] = 0;
    for(int j=0;j<max_y;j++){
  
      int16_t weight = 1;
      for(int k=0;k<cells;k++){
        weight *= matrix[k][i][j];
      }
      result[i] += weight * vector[i];
    }
  }
  return result;
}



