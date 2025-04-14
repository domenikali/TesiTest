#include <stdlib.h> 
#include <iostream>
#include <string>
#include <chrono>
#include <cstdint>
#include "../logger.hpp" 
#include "matrix.hpp"
#include <chrono>
#include <thread>

#define EXT_ERROR_CHECKS 0
#define TIMES 10000



unsigned int test_number =0;


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
//mtd: multithreaded
//std: standard
void mtd_mvm_test(int8_t ***** matrix, int8_t *vector, int sector);
void std_mvm_test(int8_t ***** matrix, int8_t * vector, int sector);
void mtd_mvm_2_test(int8_t ***** matrix, int8_t *vector, int sector);
void std_mvm_time_test(int8_t ***** matrix, int8_t *vector, int sector);
void mtd_mvm_time_test(int8_t ***** matrix, int8_t *vector, int sector);
void mtd_mvm_2_time_test(int8_t ***** matrix, int8_t *vector, int sector);
void mtd_mvm_2_1_test(int8_t ***** matrix, int8_t *vector, int sector);
void comparison_test(int8_t ***** matrix, int8_t * vector, int sector);




Logger logger("logs.txt");

int main(int args,char ** argv){

  logger.init();

  logger.log(LogLevel::INFO, "Matrix Vector Multiplication Tests");

  if(EXT_ERROR_CHECKS){
    logger.log(LogLevel::INFO, "Extencive error check enabled, all .conf file will be created");
    std::cout<<"Extencive error check enabled, all .conf file will be created"<<std::endl;
    
  }


  int8_t ***** matrix = alloc_matrix();
  int8_t * vector = new int8_t[max_vect];

  logger.log(LogLevel::INFO, "Matrix and Vector allocated");
  random_matrix(matrix);
  random_vector(vector);
  
  
  //std_mvm_test(matrix, vector,0);

  //std_mvm_time_test(matrix, vector,0);

  //mtd_mvm_test(matrix, vector,0);
    
  
  // mtd_mvm_time_test(matrix, vector,0);

  //mtd_mvm_2_test(matrix, vector,0);
  //mtd_mvm_2_time_test(matrix, vector,0);
  for(int i=0;i<100;i++){
    mtd_mvm_2_1_test(matrix, vector,0);
  }
  for(int i=0;i<100;i++){
    comparison_test(matrix, vector,0);
  }
  free_matrix(matrix);
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
void std_mvm_test(int8_t ***** matrix, int8_t *vector, int sector){
  test_number++;
  std::cout<<"Running test " << test_number << std::endl;
  logger.log(LogLevel::INFO, "Test: Standard MVM Test");
  
  auto start = std::chrono::high_resolution_clock::now();

  int64_t * result = mvm(matrix, vector,sector);
  
  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> elapsed = end - start;

  create_matrix_conf_file(matrix,sector);
  create_vector_conf_file(vector);
  create_result_conf_file(result);

  logger.log(LogLevel::INFO, "Time taken: " + std::to_string(elapsed.count()) + " seconds");

  std::cout << "Test "<<test_number<<" Competed" << std::endl;
  delete[] result;
  
}

void mtd_mvm_test(int8_t ***** matrix, int8_t *vector, int sector){
  test_number++;
  std::cout<<"Running test " << test_number << std::endl;
  logger.log(LogLevel::INFO, "Test: Multithreade MVM Test");
  
  auto start = std::chrono::high_resolution_clock::now();

  int64_t * result = mvm_multithreaded(matrix, vector,sector);
  
  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> elapsed = end - start;

  create_matrix_conf_file(matrix,sector);
  create_vector_conf_file(vector);
  create_result_conf_file(result);

 
  logger.log(LogLevel::INFO, "Time taken: " + std::to_string(elapsed.count()) + " seconds");

  
  std::cout << "Test "<<test_number<<" Competed" << std::endl;
  delete[] result;
  
}

void mtd_mvm_2_test(int8_t ***** matrix, int8_t *vector, int sector){
  test_number++;
  std::cout<<"Running test " << test_number << std::endl;

  logger.log(LogLevel::INFO, "Test: Multithreade MVM 2.0 Test");
  
  auto start = std::chrono::high_resolution_clock::now();

  int64_t * result = mvm_multithreaded_2(matrix, vector,sector);
  
  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> elapsed = end - start;

  create_matrix_conf_file(matrix,sector);
  create_vector_conf_file(vector);
  create_result_conf_file(result);

  
  logger.log(LogLevel::INFO, "Time taken: " + std::to_string(elapsed.count()) + " seconds");

  
  std::cout << "Test "<<test_number<<" Competed" << std::endl;
  delete[] result;
  
}

void std_mvm_time_test(int8_t ***** matrix, int8_t *vector, int sector){
  test_number++;
  std::cout<<"Running test " << test_number << std::endl;

  logger.log(LogLevel::INFO, "Test: Standard MVM Time Test");
  
  std::chrono::duration<double> elapsed = std::chrono::duration<double>::zero();
  for(int i=0;i<TIMES;i++){
    random_matrix(matrix);
    random_vector(vector);
    auto start = std::chrono::high_resolution_clock::now();

    int64_t * result = mvm(matrix, vector,sector);
    
    auto end = std::chrono::high_resolution_clock::now();
    elapsed += end - start;
    delete[] result;

  }


  logger.log(LogLevel::INFO, "Time taken to compelte " +std::to_string(TIMES) +" mvm with std algo: " + std::to_string(elapsed.count()/TIMES) + " seconds");
  
  std::cout << "Test "<<test_number<<" Competed" << std::endl;
  
}

void mtd_mvm_time_test(int8_t ***** matrix, int8_t *vector, int sector){
  test_number++;
  std::cout<<"Running test " << test_number << std::endl;

  logger.log(LogLevel::INFO, "Test: Multithreader MVM Time Test");
  
  std::chrono::duration<double> elapsed = std::chrono::duration<double>::zero();
  for(int i=0;i<TIMES;i++){
    random_matrix(matrix);
    random_vector(vector);
    auto start = std::chrono::high_resolution_clock::now();

    int64_t * result = mvm_multithreaded(matrix, vector,sector);
    
    auto end = std::chrono::high_resolution_clock::now();
    elapsed += end - start;
    delete[] result;

  }
  
  logger.log(LogLevel::INFO, "Time taken to compelte " +std::to_string(TIMES) +" mvm with mtd algo: " + std::to_string(elapsed.count()/TIMES) + " seconds");

  
  std::cout << "Test "<<test_number<<" Competed" << std::endl;
  
}

void mtd_mvm_2_time_test(int8_t ***** matrix, int8_t *vector, int sector){
  test_number++;
  std::cout<<"Running test " << test_number << std::endl;

  logger.log(LogLevel::INFO, "Test: Multithreader MVM 2.0 Time Test");
  
  std::chrono::duration<double> elapsed = std::chrono::duration<double>::zero();
  for(int i=0;i<TIMES;i++){
    random_matrix(matrix);
    random_vector(vector);
    auto start = std::chrono::high_resolution_clock::now();

    int64_t * result = mvm_multithreaded_2(matrix, vector,sector);
    
    auto end = std::chrono::high_resolution_clock::now();
    elapsed += end - start;
    delete[] result;
    
  }
  

  
  logger.log(LogLevel::INFO, "Time taken to compelte " +std::to_string(TIMES) +" mvm with mtd algo: " + std::to_string(elapsed.count()/TIMES) + " seconds");

  
  std::cout << "Test "<<test_number<<" Competed" << std::endl;
  
}

void mtd_mvm_2_1_test(int8_t ***** matrix, int8_t *vector, int sector){
  test_number++;
  std::cout<<"Running test " << test_number << std::endl;

  logger.log(LogLevel::INFO, "Test: Multithreade MVM 2.1 Test");
  
  auto start = std::chrono::high_resolution_clock::now();

  int64_t * result = mvm_multithreaded_2_1(matrix, vector,sector);
  
  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> elapsed = end - start;

  create_matrix_conf_file(matrix,sector);
  create_vector_conf_file(vector);
  create_result_conf_file(result);

  
  logger.log(LogLevel::INFO, "Time taken: " + std::to_string(elapsed.count()) + " seconds");

  
  std::cout << "Test "<<test_number<<" Competed" << std::endl;
  delete[] result;
  
}

void comparison_test(int8_t ***** matrix, int8_t * vector, int sector){
  test_number++;
  std::cout<<"Running test " << test_number << std::endl;

  logger.log(LogLevel::INFO, "Test: Comparison Test");

  random_matrix(matrix);
  random_vector(vector);
  auto start = std::chrono::high_resolution_clock::now();
  int64_t * result = mvm(matrix, vector,sector);
  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> elapsed = end - start;
  logger.log(LogLevel::INFO, "Time taken for standard mvm: " + std::to_string(elapsed.count()) + " seconds");
  auto start_1 = std::chrono::high_resolution_clock::now();
  int64_t * result_2 = mvm_multithreaded(matrix, vector,sector);
  auto end_1 = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> elapsed_1 = end_1 - start_1;
  logger.log(LogLevel::INFO, "Time taken for multithreaded mvm: " + std::to_string(elapsed_1.count()) + " seconds");
  auto start_2 = std::chrono::high_resolution_clock::now();
  int64_t * result_3 = mvm_multithreaded_2(matrix, vector,sector);
  auto end_2 = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> elapsed_2 = end_2 - start_2;
  logger.log(LogLevel::INFO, "Time taken for multithreaded mvm 2.0: " + std::to_string(elapsed_2.count()) + " seconds");
  auto start_3 = std::chrono::high_resolution_clock::now();
  int64_t * result_4 = mvm_multithreaded_2_1(matrix, vector,sector);
  auto end_3 = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> elapsed_3 = end_3 - start_3;
  logger.log(LogLevel::INFO, "Time taken for multithreaded mvm 2.1: " + std::to_string(elapsed_3.count()) + " seconds");


  std::cout << "Test "<<test_number<<" Competed" << std::endl;
  delete[] result;
  delete[] result_2;
  delete[] result_3;
  delete[] result_4;

}

