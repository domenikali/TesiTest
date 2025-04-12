#include <stdlib.h> 
#include <iostream>
#include <string>
#include <chrono>
#include <cstdint>
#include "../logger.hpp" 
#include "matrix.hpp"
#include <chrono>
#include <thread>

std::string pretty_vector_64(int64_t * vector, int size){
  std::string result = "[";
  for(int i=0;i<size;i++){
    result += std::to_string(vector[i]);
    if(i != size-1){
      result += ", ";
    }
  }
  result += "]";
  return result;
}

std::string pretty_vector_8(int8_t * vector, int size){
  std::string result = "[";
  for(int i=0;i<size;i++){
    result += std::to_string(vector[i]);
    if(i != size-1){
      result += ", ";
    }
  }
  result += "]";
  return result;
}




  
//tests singnatures:
bool tes1(int8_t ***** matrix, int8_t * vector);
bool timeTest(int8_t ***** matrix, int8_t * vector,int8_t init);
bool mulitTimeTest(int8_t ***** matrix, int8_t * vector);
bool mulitCellTimeTest(int8_t ***** matrix, int8_t * vector);
bool multiCellTest(int8_t ***** matrix, int8_t * vector);
bool mvm_test(int8_t ***** matrix, int8_t * vector, int sector);


Logger logger("logs.txt");

int main(int args,char ** argv){

  logger.init();

  logger.log(LogLevel::INFO, "Matrix Vector Multiplication Tests");

  int8_t ***** matrix = alloc_matrix();
  int8_t * vector = new int8_t[max_vect];

  logger.log(LogLevel::INFO, "Matrix and Vector allocated");
  random_matrix(matrix);
  random_vector(vector);
  
  
  std::cout<<"Test 1 running.."<<std::endl;
  bool res = mvm_test(matrix, vector,0);
  std::cout << "Test 1 completed:"<< std::string(res ? "Passed" : "Failed") << std::endl;
  logger.log(LogLevel::INFO, "Test 1 completed: " + std::string(res ? "Passed" : "Failed"));
  

  delete[] vector;
  logger.log(LogLevel::INFO, "Matrix Vector Multiplication Tests Completed");
  return 0;
}

/**@simple mvm test
 * make a computation, count the time elaped during the computation then create logs file for input vector, matrix and result readable from an external matrix script to check the result accuracy
 * then log everything on the txt log file 
 * lastly delalocate the result vector
 * @param matrix the matrix to be used for the computation
 * @param vector the input vector to be used for the computation
 * @param sector the sector of the matrix to be used for the computation
 * TODO: make the sectors indipendant 
 */
bool mvm_test(int8_t ***** matrix, int8_t *vector, int sector){
  logger.log(LogLevel::INFO, "Test: MVM Test");
  
  auto start = std::chrono::high_resolution_clock::now();

  int64_t * result = mvm_multithreaded(matrix, vector,sector);
  
  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> elapsed = end - start;

  create_matrix_conf_file(matrix,sector);
  create_vector_conf_file(vector);
  create_result_conf_file(result);

  logger.log(LogLevel::INFO, "vector: " + pretty_vector_8(vector, max_vect));
  //log.log(LogLevel::INFO, "Matrix: " + preatty_matrix(matrix));
  //logger.log(LogLevel::INFO, "result: " + pretty_vector_64(result, max_vect));
  logger.log(LogLevel::INFO, "Time taken: " + std::to_string(elapsed.count()) + " seconds");

  
  std::cout << "mvm test passed" << std::endl;
  delete[] result;
  
  return true;
}
